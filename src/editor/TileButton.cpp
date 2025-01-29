#include "TileButton.hpp"

TileButton::TileButton(QWidget *parent, const uint8_t id) : QPushButton(parent){
    QPixmap pixmap = SpriteProvider::get_sprite(id);
    QPixmap scaledpixmap = pixmap.scaled(72,72, Qt::KeepAspectRatio, Qt::FastTransformation);
    this -> setIcon(QIcon(scaledpixmap));
    this -> setIconSize(QSize(72,72));
    this -> setFixedSize(80,80);
}

void TileButton::mousePressEvent(QMouseEvent *event){
    QPushButton::mousePressEvent(event);
    sendTileSelected(id);
}
