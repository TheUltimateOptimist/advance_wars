/**
* LevelView.hpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QGraphicsView>

#include "LevelScene.hpp"
#include "EventHandler.hpp"

namespace editor
{

/**
 * The LevelView Widget is responsible for holding the LevelScene.
 * It enables scrolling by default in case the LevelScene does not fit on 
 * the entire screen.
 * It's also responsible for zooming the LevelScene in case a Zoom Event is received.
 * To receive Zoom Events LevelView inherits from EventHandler and overwrites onZoomed
 */
class LevelView : public QGraphicsView, public EventHandler{
public:
    /**
     * Constructs a LevelView widget from the given scene and parent.
     * @param scene The level scene that should be contained in this view.
     * @param parent The widget to set as this widget's parent.
     */
    LevelView(LevelScene* scene, QWidget* parent = nullptr);

private:
    /**
     * The current value by which the contained scene is scaled up or down.
     * E.g 2.0 => twice the size, 0.5 => half the size
     */
    double scale_val;

    /**
     * Zoom by the specified delta.
     * @param delta The amount by which to zoom.
     */
    void onZoomed(double delta) override;
};

} // namespace editor