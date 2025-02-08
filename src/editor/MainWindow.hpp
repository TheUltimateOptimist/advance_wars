/**
* MainWindow.hpp
*
* @date 27.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QMainWindow>

#include "LevelScene.hpp"
#include "EventHandler.hpp"

namespace editor 
{

/**
 * MainWindow is the top level Widget of the LevelEditor. 
 * All other Widgets are children of MainWindow. 
 * MainWindow is instantiated on the stack in main.cpp.
 * Thus, MainWindow is freed automatically at application exit together
 * with all of the other widgets (descendants), because of the way memory management works in QT.
 * MainWindow inherits from EventHandler so that it can override onLevelNameUpadated in order
 * to change the window title when the level name is changed.
 */
class MainWindow : public QMainWindow, public EventHandler {
public:
    /**
     * Creates a MainWindow from the given LevelScene and parent.
     * @param level The LevelScene of the level that should be edited.
     * @param parent The parent widget for the MainWindow.
     */
    MainWindow(LevelScene* level, QWidget *parent = nullptr);

private:
    /**
     * Updates the Window Title with the new_name.
     * @param new_name The new level name.
     */
    void onLevelNameUpdated(std::string new_name) override;

    /**
     * The widht of the level that is being edited.
     * Unit: number of tiles.
     */
    int m_level_width;

    /**
     * The height of the level that is being edited.
     * Unit: number of tiles.
     */ 
    int level_height;
};

} // namespace editor