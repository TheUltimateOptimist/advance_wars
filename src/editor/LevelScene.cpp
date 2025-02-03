#include "LevelScene.hpp"
#include "SpriteProvider.hpp"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPoint>

#include "highfive/H5File.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace editor
{

LevelScene::LevelScene(
    const std::string& name, int width, int height, std::vector<uint8_t> tile_ids,
    const std::string& file_path, QWidget* parent)
    : QGraphicsScene(parent), m_name(name), m_width(width + 2), m_height(height + 2), m_tile_ids(tile_ids),
      m_file_path(file_path), m_selected_tile_id(2)
{
    for (int h = 0; h < m_height; h++) {
    for (int i = 0; i < m_width; i++) {
        std::cout << (int) tile_ids[m_width*h + i] << " ";
    }
    std::cout << std::endl;
    }

    setSceneRect(0, 0, m_width * 16, m_height * 16);
    m_tile_occupants = {};
    m_tile_occupants.reserve(tile_ids.size());
    for (int index = 0; index < tile_ids.size(); index++)
    {
        int   x = (index % m_width) * 16;
        int   y = (index / m_width) * 16;
        Tile* tile = new Tile(index, is_border(index) ? tile_ids[index] : 0);
        addItem(tile);
        tile->setZValue(0);
        tile->setPos(x, y);
        if (!is_border(index) && tile_ids[index] > 0)
        {
            m_tile_occupants.push_back(occupy_tile(index, tile_ids[index]));
        }
        else
        {
            m_tile_occupants.push_back(nullptr);
        }
    }
}

LevelScene* LevelScene::empty(const std::string& name, int width, int height, QWidget* parent)
{
    // + 2 because of surrounding the map with cliffs
    std::vector<uint8_t> tile_ids;
    tile_ids.reserve((width + 2)*(height + 2));

    // create top row with cliffs
    tile_ids.push_back(22); // cliff corner top left 
    for (int i = 0; i < width; i++) {
        tile_ids.push_back(19); // cliff bottom
    }
    tile_ids.push_back(23); // cliff corner top right 

    // create main rows with cliff at start and end
    for (int i = 0; i < height; i++) {
        tile_ids.push_back(21); // cliff right
        for (int j = 0; j < width; j++) {
            tile_ids.push_back(0); // pleins
        }
        tile_ids.push_back(20); // cliff left
    }

    // create bottom row with cliffs
    tile_ids.push_back(24); // cliff corner bottom left 
    for (int i = 0; i < width; i++) {
        tile_ids.push_back(18); // cliff top
    }
    tile_ids.push_back(25); // cliff corner bottom right 

    return new LevelScene(name, width, height, tile_ids, "../res/level_new.h5", parent);
}

LevelScene* LevelScene::fromFile(const std::string& file_path, QWidget* parent)
{
    HighFive::File file(file_path, HighFive::File::ReadOnly);

    // read level metadata
    std::string level_metadata;
    file.getDataSet("metadata").read(level_metadata);

    // read tilesarray
    std::vector<uint8_t> level_tilesarray;
    file.getDataSet("tilesarray").read(level_tilesarray);

    // extract metadata from xml
    std::istringstream          xmlStream(level_metadata);
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(xmlStream, pt);
    int         width = pt.get<int>("level.width");
    int         height = pt.get<int>("level.height");
    std::string name = pt.get<std::string>("level.name");

    return new LevelScene(name, width, height, level_tilesarray, file_path, parent);
}

std::string LevelScene::getName()
{
    return m_name;
}

int LevelScene::getWidth()
{
    return m_width - 2;
}

int LevelScene::getHeight()
{
    return m_height - 2;
}

bool LevelScene::is_border(int index)
{
    return (index / m_width) == 0 
    || (index / m_width) == (m_height -1)
    || (index % m_width) == 0 
    || (index % m_width) == (m_width - 1);
}

bool LevelScene::is_water_tile(uint8_t id)
{
    if (id == 1) 
    {
        return true;
    }
    if (id >= 17 && id <= 29)
    {
        return true;
    }
    return false;
}

void LevelScene::onLevelNameUpdated(std::string new_name)
{
    m_name = new_name;
}

void LevelScene::onLevelWriteRequested(QString file_path)
{
    boost::property_tree::ptree pt;

    // Add data to the property tree
    pt.put("level.width", m_width - 2);
    pt.put("level.height", m_height - 2);
    pt.put("level.name", m_name);

    // convert property tree to xml string
    std::ostringstream xmlStream;
    boost::property_tree::write_xml(xmlStream, pt);
    std::string xml_data = xmlStream.str();

    // write level to hdf5
    HighFive::File file(file_path.toStdString(), HighFive::File::Truncate);
    file.createDataSet<std::string>("metadata", HighFive::DataSpace::From(xmlStream))
        .write(xml_data);
    file.createDataSet<uint8_t>("tilesarray", HighFive::DataSpace::From(m_tile_ids))
        .write(m_tile_ids);
}

void LevelScene::onTileEntered(int index)
{
    if (m_selected_tile_id == m_tile_ids[index])
    {
        return;
    }
    if (is_border(index) && !is_water_tile(m_selected_tile_id))
    {
        return;
    }
    if (m_tile_occupants[index] != nullptr)
    {
        removeItem(m_tile_occupants[index]);
        m_tile_occupants[index] = nullptr;
    }
    if (!is_border(index) && m_selected_tile_id > 0 || is_border(index))
    {
        m_tile_occupants[index] = occupy_tile(index, m_selected_tile_id);
    }
}

void LevelScene::onTileExited(int index)
{
    if (m_selected_tile_id == m_tile_ids[index])
    {
        return;
    }
    if (m_tile_occupants[index] != nullptr)
    {
        removeItem(m_tile_occupants[index]);
        m_tile_occupants[index] = nullptr;
    }
    if (!is_border(index) && m_tile_ids[index] > 0 || is_border(index))
    {
        m_tile_occupants[index] = occupy_tile(index, m_tile_ids[index]);
    }
}

void LevelScene::onTileClicked(int index)
{
    m_tile_ids[index] = m_selected_tile_id;
}

void LevelScene::onNewTileIdSelected(uint8_t tile_id)
{
    m_selected_tile_id = tile_id;
}

QGraphicsPixmapItem* LevelScene::occupy_tile(int index, uint8_t tile_id)
{
    int                  x = (index % m_width) * 16;
    int                  y = (index / m_width) * 16;
    QPixmap              tile_occupant = SpriteProvider::get_sprite(tile_id);
    QGraphicsPixmapItem* tile_occupant_item = addPixmap(tile_occupant);
    tile_occupant_item->setZValue(tile_id < 50 ? 1 : 2 + index);
    tile_occupant_item->setPos(x, tile_id < 50 ? y : y - 16);
    return tile_occupant_item;
}

} // namespace editor