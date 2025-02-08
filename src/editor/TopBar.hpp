/**
* TopBar.hpp
*
* @date 28.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QToolBar>

namespace editor
{

/**
 * The TopBar provides key functionality for the LevelEditor:
 * - LineEdit for setting the level name
 * - CheckBox for toggling advanced placement on and off
 * - ZoomInButton
 * - ZoomOutButton
 * - ZoomInfo (displays current zoom percentage)
 * - SaveButon
 * TopBar inherits from QToolBar to ensure the QT look and fell.
 */
class TopBar : public QToolBar {
public: 
    /**
     * Creates a TopBar with the provided level_name and parent.
     * The level_name is needed to prefill the LineEdit in the TopBar with
     * the current level name.
     * @param level_name The current name of the level.
     * @param parent The parent to set for the TopBar.
     */
    TopBar(const std::string& level_name, QWidget *parent = nullptr);
};

} // namespace editor