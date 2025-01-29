#include "LevelScene.hpp"
#include "SpriteProvider.hpp"
#include <QGraphicsPixmapItem>
#include <iostream>

LevelScene::LevelScene(const std::string& name, int width, int height, uint8_t* tiles, QWidget *parent) : QGraphicsScene(parent), name(name), width(width), height(height), tiles(tiles) {
    this->setSceneRect(0, 0, width*16, height*16 + 32);
    QPixmap plein = SpriteProvider::get_sprite(0);
    for (int index = 0; index < width*height; index++) {
        std::cout << "Tile index: " << index << std::endl;
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



LevelScene::~LevelScene() {
    delete tiles;
}

LevelScene *LevelScene::empty(const std::string& name, int width, int height, QWidget *parent) {
    uint8_t* tiles = new uint8_t[width*height];
    for (int i = 0; i < width*height; i++) {
        tiles[i] = 0;
    }
    return new LevelScene(name, width, height, tiles, parent);
}

void LevelScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF clickPosition = event->scenePos();
    int x = (int) clickPosition.rx() / 16;
    int y = (int) clickPosition.ry() / 16 - 1;
    int tile_index = y * width + x;
    qDebug() << "Mouse clicked at:" << clickPosition;
    qDebug() << "Tile index:" << tile_index;
    qDebug() << "x" << x << "y" << y;
    if (tiles[tile_index] != 0) {
        QGraphicsPixmapItem* item = (QGraphicsPixmapItem*) this->itemAt(clickPosition, QTransform());
        this->removeItem(item);
    }
    QPixmap new_pixmap = SpriteProvider::get_sprite(50);
    QGraphicsPixmapItem* new_item = this->addPixmap(new_pixmap);
    new_item->setPos(x*16, y*16);
    tiles[tile_index] = 50;
    QGraphicsScene::mousePressEvent(event);
}

void LevelScene::onLevelNameUpdated(std::string new_name)
{
    std::cout << "Name Updated " << new_name << std::endl;
    this->name = new_name;
}

void LevelScene::onLevelWriteRequested()
{
    std::cout << "Write Requested" << std::endl;
}
