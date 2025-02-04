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

class MainWindow : public QMainWindow, public EventHandler {
public:
    MainWindow(LevelScene* level, QWidget *parent = nullptr);
private:
    void onLevelNameUpdated(std::string new_name) override;
    int m_level_width;
    int level_height;
};

} // namespace editor