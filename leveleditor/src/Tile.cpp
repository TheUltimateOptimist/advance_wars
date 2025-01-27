#include "Tile.hpp"
#include <iostream>

Tile::Tile(const QPixmap &pixmap, uint8_t id) : QGraphicsPixmapItem(pixmap) {}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    std::cout << "clicked" << std::endl;
}
