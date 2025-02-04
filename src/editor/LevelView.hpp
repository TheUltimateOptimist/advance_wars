/**
* LevelView.hpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QGraphicsView>
#include "LevelScene.hpp"
#include "EventBroker.hpp"

namespace editor
{

class LevelView : public QGraphicsView, public EventBroker{
public:
    LevelView(LevelScene* scene, QWidget* parent = nullptr);
private:
    double scale_val;
    void onZoomed(double delta) override;
};

} // namespace editor