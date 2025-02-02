#include "LevelScene.hpp"
#include "SpriteProvider.hpp"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPoint>

#include "highfive/H5File.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

LevelScene::LevelScene(const std::string& name, int width, int height, std::vector<uint8_t> tile_ids, const std::string& file_path, QWidget *parent) : QGraphicsScene(parent), name(name), width(width), height(height), tile_ids(tile_ids), file_path(file_path), selected_tile_id(2) {
    this->setSceneRect(0, 0, width*16 + 32, height*16 + 32);
    std::vector<QGraphicsPixmapItem*> tile_occupants;
    QPixmap cliff_left = SpriteProvider::get_sprite(20);
    QPixmap cliff_right = SpriteProvider::get_sprite(21);
    QPixmap cliff_top = SpriteProvider::get_sprite(18);
    QPixmap cliff_bottom = SpriteProvider::get_sprite(19);
    QPixmap cliff_corner_top_left = SpriteProvider::get_sprite(22);
    QPixmap cliff_corner_top_right = SpriteProvider::get_sprite(23);
    QPixmap cliff_corner_bottom_left = SpriteProvider::get_sprite(24);
    QPixmap cliff_corner_bottom_right = SpriteProvider::get_sprite(25);
    for (int index = 1; index <= height; index++) {
        addPixmap(cliff_right)->setPos(0, index*16);
        addPixmap(cliff_left)->setPos(width*16 + 16, index*16);
    }
    for (int index = 1; index <= width; index++) {
        addPixmap(cliff_bottom)->setPos(index*16, 0);
        addPixmap(cliff_top)->setPos(index*16, height*16 + 16);
    }
    addPixmap(cliff_corner_top_left)->setPos(0, 0);
    addPixmap(cliff_corner_top_right)->setPos(width*16 + 16, 0);
    addPixmap(cliff_corner_bottom_left)->setPos(0, height*16 + 16);
    addPixmap(cliff_corner_bottom_right)->setPos(width*16 + 16, height*16 + 16);
    for (int index = 0; index < width*height; index++) {
        int x = (index % width) * 16 + 16;
        int y = (index / width) * 16 + 16;
        Tile* tile = new Tile(index);
        this->addItem(tile);
        tile->setZValue(0);
        tile->setPos(x, y);
        if (tile_ids[index] > 0) {
            QPixmap tile_occupant = SpriteProvider::get_sprite(tile_ids[index]);
            QGraphicsPixmapItem* tile_occupant_item = addPixmap(tile_occupant);
            tile_occupant_item->setZValue(tile_ids[index] < 50 ? 1 : 2 + index);
            tile_occupant_item->setPos(x, tile_ids[index] < 50 ? y : y - 16);
            tile_occupants.push_back(tile_occupant_item);
        }
        else {
            tile_occupants.push_back(nullptr);
        }
    }
    this->tile_occupants = tile_occupants;
}

LevelScene *LevelScene::empty(const std::string& name, int width, int height, QWidget *parent) {
    std::vector<uint8_t> tile_ids(width*height); 
    for (int i = 0; i < width*height; i++) {
        tile_ids[i] = 0;
    }
    return new LevelScene(name, width, height, tile_ids, "../res/level_new.h5", parent);
}

LevelScene *LevelScene::fromFile(const std::string &file_path, QWidget *parent)
{
    HighFive::File file(file_path, HighFive::File::ReadOnly);

    // read level metadata
    std::string level_metadata;
    file.getDataSet("metadata").read(level_metadata);

    // read tilesarray
    std::vector<uint8_t> level_tilesarray;
    file.getDataSet("tilesarray").read(level_tilesarray);

    // extract metadata from xml
    std::istringstream xmlStream(level_metadata);
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(xmlStream, pt);
    int width = pt.get<int>("level.width");
    int height = pt.get<int>("level.height");
    std::string name = pt.get<std::string>("level.name");

    return new LevelScene(name, width, height, level_tilesarray, file_path, parent);
}

std::string LevelScene::getName()
{
    return name;
}

int LevelScene::getWidth()
{
    return width;
}

int LevelScene::getHeight()
{
    return height;
}

void LevelScene::onLevelNameUpdated(std::string new_name)
{
    this->name = new_name;
}

void LevelScene::onLevelWriteRequested(QString file_path)
{
    boost::property_tree::ptree pt;

    // Add data to the property tree
    pt.put("level.width", width);
    pt.put("level.height", height);
    pt.put("level.name", name);

    // convert property tree to xml string
    std::ostringstream xmlStream;
    boost::property_tree::write_xml(xmlStream, pt);
    std::string xml_data = xmlStream.str();

    // write level to hdf5
    HighFive::File file(file_path.toStdString(), HighFive::File::Truncate);
    file.createDataSet<std::string>("metadata", HighFive::DataSpace::From(xmlStream)).write(xml_data);
    file.createDataSet<uint8_t>("tilesarray", HighFive::DataSpace::From(tile_ids)).write(tile_ids);
}

void LevelScene::onTileEntered(int index)
{
    if (selected_tile_id == tile_ids[index]) return;
    if (tile_occupants[index] != nullptr) removeItem(tile_occupants[index]);
    tile_occupants[index] = nullptr;
    if (selected_tile_id > 0) {
        tile_occupants[index] = occupy_tile(index, selected_tile_id);
    } 
}

void LevelScene::onTileExited(int index)
{
    if (selected_tile_id == tile_ids[index]) return;
    if (tile_occupants[index] != nullptr) removeItem(tile_occupants[index]);
    tile_occupants[index] = nullptr;
    if (tile_ids[index] > 0) {
        tile_occupants[index] = occupy_tile(index, tile_ids[index]);
    }
}

void LevelScene::onTileClicked(int index)
{
    tile_ids[index] = selected_tile_id;
}

void LevelScene::onNewTileIdSelected(uint8_t tile_id)
{
    this->selected_tile_id = tile_id;
}

QGraphicsPixmapItem *LevelScene::occupy_tile(int index, uint8_t tile_id)
{
    if (tile_id == 0) return nullptr;
    int x = (index % width) * 16 + 16;
    int y = (index / width) * 16 + 16;
    QPixmap tile_occupant = SpriteProvider::get_sprite(tile_id);
    QGraphicsPixmapItem* tile_occupant_item = addPixmap(tile_occupant);
    tile_occupant_item->setZValue(tile_id < 50 ? 1 : 2 + index);
    tile_occupant_item->setPos(x, tile_id < 50 ? y : y - 16);    
    return tile_occupant_item;
}
