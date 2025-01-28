#include "TileSelector.hpp"

TileSelector::TileSelector(QWidget * parent) : QScrollArea(parent){
    this->setFixedWidth(300);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}
