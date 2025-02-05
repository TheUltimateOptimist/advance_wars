/**
* LevelView.cpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "LevelView.hpp"

#include "SpriteProvider.hpp"

namespace editor
{

LevelView::LevelView(LevelScene* scene, QWidget* parent) : QGraphicsView(parent), scale_val(2)
{ 
    scene->setParent(this);
    setScene(scene);
    setAlignment(Qt::AlignCenter);
    scale(scale_val, scale_val); // view starts at 2x zoom by default
    setBackgroundBrush(QBrush(SpriteProvider::get_sprite(1)));
}

void LevelView::onZoomed(double delta)
{
    // amount by which to scale has to be calculated using scale_vale (how much was already zoomed before)
    // and delta (desired zoom percentage)
    double scale_by = (scale_val + delta) / scale_val;
    if (scale_by <= 0)
    {
        return;
    }
    scale_val += delta;
    scale(scale_by, scale_by);
}

} // namespace editor