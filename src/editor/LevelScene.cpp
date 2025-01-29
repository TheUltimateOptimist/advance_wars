#include "LevelScene.hpp"
#include "SpriteProvider.hpp"
#include <QGraphicsPixmapItem>

#include "highfive/H5File.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

LevelScene::LevelScene(const std::string& name, int width, int height, std::vector<uint8_t> tiles, const std::string& file_path, QWidget *parent) : QGraphicsScene(parent), name(name), width(width), height(height), tiles(tiles), file_path(file_path) {
    this->setSceneRect(0, 0, width*16, height*16 + 32);
    QPixmap plein = SpriteProvider::get_sprite(0);
    for (int index = 0; index < width*height; index++) {
        uint8_t tile_id = tiles[index];
        int x = (index % width) * 16;
        int y = (index / width) * 16 + 16;
        QGraphicsPixmapItem* plein_item = this->addPixmap(plein);
        plein_item->setPos(x, y);
        plein_item->setZValue(0);
        if (tile_id > 0) {
            QPixmap pixmap = SpriteProvider::get_sprite(tile_id);
            QGraphicsPixmapItem* pixmap_item = this->addPixmap(pixmap);
            if (tile_id <= 29) {
                pixmap_item->setPos(x, y);
            } else {
                pixmap_item->setPos(x, y - 16);
            }
            if (tile_id <= 29) {
                pixmap_item->setZValue(1);
            } else {
                pixmap_item->setZValue(y);
            }
        }
    }
}

LevelScene *LevelScene::empty(const std::string& name, int width, int height, QWidget *parent) {
    std::vector<uint8_t> tiles(width*height); 
    for (int i = 0; i < width*height; i++) {
        tiles[i] = 0;
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

    return new LevelScene(name, width, height, level_tilesarray, file_path, parent);
}

void LevelScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
    QPointF clickPosition = event->scenePos();
    int x = (int) clickPosition.rx() / 16;
    int y = (int) clickPosition.ry() / 16 - 1;
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return;
    }
    int tile_index = y * width + x;
    if (tiles[tile_index] != 0) {
        QGraphicsPixmapItem* item = (QGraphicsPixmapItem*) this->itemAt(clickPosition, QTransform());
        this->removeItem(item);
    }
    QPixmap new_pixmap = SpriteProvider::get_sprite(50);
    QGraphicsPixmapItem* new_item = this->addPixmap(new_pixmap);
    new_item->setPos(x*16, y*16);
    new_item->setZValue(y*16);
    tiles[tile_index] = 50;
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

    std::ostringstream xmlStream;
    boost::property_tree::write_xml(xmlStream, pt);
    std::string xml_data = xmlStream.str();
    HighFive::File file(file_path, HighFive::File::Truncate);
    file.createDataSet<std::string>("metadata", HighFive::DataSpace::From(xmlStream)).write(xml_data);
    file.createDataSet<uint8_t>("tilesarray", HighFive::DataSpace::From(tiles)).write(tiles);
}
