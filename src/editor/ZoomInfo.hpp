/**
* ZoomInfo.cpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QPushButton>

#include "EventHandler.hpp"

namespace editor
{

/**
 * ZoomInfo inherits from QPushButton, but doesn't actually act as a button. 
 * It just displays the current zoom percentage.
 * To update the text that is displayed in case of a zoom change, it 
 * inherits from EventHandler and overwrites the onZoomed event method.
 */
class ZoomInfo : public QPushButton, public EventHandler {
public:
    ZoomInfo(QWidget* parent = nullptr);
private:
    /**
     * The percentage by which the LevelMap is zoomed in/out at the moment.
     */
    double current_scale;

    /**
     * Updates the zoom info text to correctly display the updated zoom percentage.
     * @param delta The percentage to add/subtract to the current zoom percentage.
     */
    void onZoomed(double delta) override;
};

} // namespace editor