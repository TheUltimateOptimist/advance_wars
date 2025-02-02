#include "LevelView.hpp"
#include "SpriteProvider.hpp"

LevelView::LevelView(LevelScene *scene, QWidget *parent) : QGraphicsView(parent), scale_val(2)
{
    scene->setParent(this);
    this->setScene(scene);
    this->setAlignment(Qt::AlignCenter);
    this->scale(scale_val, scale_val);
    this->setBackgroundBrush(QBrush(SpriteProvider::get_sprite(1)));

}

void LevelView::onZoomed(double delta)
{

    double scale_by = (scale_val + delta) / scale_val;
    if (scale_by <= 0) return;
    scale_val += delta;
    scale(scale_by, scale_by);
}
