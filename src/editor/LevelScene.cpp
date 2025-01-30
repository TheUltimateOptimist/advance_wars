#include "LevelScene.hpp"
#include "SpriteProvider.hpp"
#include <QGraphicsPixmapItem>

#include "highfive/H5File.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

LevelScene::LevelScene(const std::string& name, int width, int height, std::vector<Tile*> tiles, const std::string& file_path, QWidget *parent) : QGraphicsScene(parent), name(name), width(width), height(height), tiles(tiles), file_path(file_path), active_tile(nullptr), active_tile_marker(nullptr), hovered_tile_marker(nullptr) {
    this->setSceneRect(0, 0, width*16, height*16);
    for (int index = 0; index < width*height; index++) {
        int x = (index % width) * 16;
        int y = (index / width) * 16;
        Tile* tile = tiles[index];
        this->addItem(tile);
        tile->setPos(x, y);
        if (tile->getId() > 0) {
            this->addItem(tile->getChild());
            tile->getChild()->setPos(x, y);
        }
        if (tile->getId() >= 50) {
            tile->getChild()->setPos(x, y - 16);
            tile->getChild()->setZValue(2 + index);
        }
    }
}

LevelScene *LevelScene::empty(const std::string& name, int width, int height, QWidget *parent) {
    std::vector<Tile*> tiles(width*height); 
    for (int i = 0; i < width*height; i++) {
        tiles[i] = new Tile(0);
    }
    return new LevelScene(name, width, height, tiles, "../res/level_new.h5", parent);
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

    std::vector<Tile*> tiles;
    for (int i = 0; i < width*height; i++) {
        tiles.push_back(new Tile(level_tilesarray[i]));
    }

    return new LevelScene(name, width, height, tiles, file_path, parent);
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

void LevelScene::onLevelWriteRequested()
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


    // create tiles_array
    std::vector<uint8_t> tiles_array;
    for (int i = 0; i < width*height; i++) {
        tiles_array.push_back(tiles[i]->getId());
    }

    // write level to hdf5
    HighFive::File file(file_path, HighFive::File::Truncate);
    file.createDataSet<std::string>("metadata", HighFive::DataSpace::From(xmlStream)).write(xml_data);
    file.createDataSet<uint8_t>("tilesarray", HighFive::DataSpace::From(tiles_array)).write(tiles_array);
}

void LevelScene::onTileEntered(Tile *tile)
{
    QGraphicsRectItem* marker = createMarkerOn(tile);
    hovered_tile_marker = marker;
}

void LevelScene::onTileExited(Tile *tile)
{
    if (hovered_tile_marker != nullptr) {
        this->removeItem(hovered_tile_marker);
    }
}

void LevelScene::onTileClicked(Tile *tile)
{
    if (active_tile_marker != nullptr) {
        this->removeItem(active_tile_marker);
    }
    active_tile = tile;
    QGraphicsRectItem* marker = createMarkerOn(tile);
    active_tile_marker = marker;
}

void LevelScene::onTileSelected(uint8_t id) {
    if (active_tile == nullptr) return;
    active_tile->setId(id);
    if (active_tile->getChild() != nullptr) {
        this->removeItem(active_tile->getChild());
    }
    if (id == 0) {
        active_tile->setChild(nullptr);
    }
    else {
        QPixmap new_pixmap = SpriteProvider::get_sprite(id);
        QGraphicsPixmapItem* new_item = this->addPixmap(new_pixmap);
        new_item->setPos(active_tile->x(), active_tile->y());
        new_item->setZValue(1);
        active_tile->setChild(new_item);
        if (id >= 50) {
            new_item->setPos(active_tile->x(), active_tile->y() - 16);
            new_item->setZValue(2 + width*(active_tile->y()/16) + active_tile->x()/16);
        }
    }
}

QGraphicsRectItem *LevelScene::createMarkerOn(Tile *tile)
{
    QColor marker_color(0, 0, 0, 128);
    QPen marker_pen(Qt::transparent);;
    QBrush marker_brush(marker_color);
    QGraphicsRectItem* focus_rect_item = this->addRect(0, 0, 16, 16, marker_pen, marker_brush);
    focus_rect_item->setZValue(std::numeric_limits<qreal>::max());
    focus_rect_item->setPos(tile->x(), tile->y());
    return focus_rect_item;
}
