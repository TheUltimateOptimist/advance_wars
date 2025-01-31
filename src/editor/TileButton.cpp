#include "SpriteProvider.hpp"
#include "TileButton.hpp"
#include "EventBroker.hpp"

TileButton::TileButton(const uint8_t id, QWidget *parent) : QPushButton(parent), id(id){
    QPixmap pixmap = SpriteProvider::get_sprite(id);
    QPixmap scaledpixmap = pixmap.scaled(72,72, Qt::KeepAspectRatio, Qt::FastTransformation);
    this -> setIcon(QIcon(scaledpixmap));
    this -> setIconSize(QSize(72,72));
    this -> setFixedSize(80,80);
}

void TileButton::mousePressEvent(QMouseEvent *event){
    QPushButton::mousePressEvent(event);
    uint8_t tile_id = id;
    EventBroker::send([tile_id](EventBroker* e){e->onNewTileIdSelected(tile_id);});
}
