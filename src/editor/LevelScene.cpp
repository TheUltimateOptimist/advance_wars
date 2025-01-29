#include "LevelScene.hpp"
#include "SpriteProvider.hpp"
#include <QGraphicsPixmapItem>

LevelScene::LevelScene(int width, int height, QWidget *parent) : QGraphicsScene(parent){
    this->setSceneRect(0, 0, width*16, height*16 + 32);
    SpriteProvider tileprovider = SpriteProvider::from_spritesheet("../res/spritesheet.h5");
    int index = 0;
    for (int y = 16; y < height*16 + 32; y+=16) {
        for (int x = 0; x < width*16; x+=16) {
            QPixmap pixmap = tileprovider.get_sprite((index % 2)*50);
            QGraphicsPixmapItem* pixmap_item = this->addPixmap(pixmap);
            pixmap_item->setPos(x, y);
            index++;
        }
    }
}

void LevelScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickPosition = event->scenePos();
    
    qDebug() << "Mouse clicked at:" << clickPosition;
    QGraphicsScene::mousePressEvent(event);
}
