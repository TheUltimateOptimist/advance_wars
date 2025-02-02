#pragma once

#include <QMainWindow>
#include "LevelScene.hpp"
#include "EventBroker.hpp"

namespace editor 
{

class MainWindow : public QMainWindow, public EventBroker {
public:
    MainWindow(LevelScene* level, QWidget *parent = nullptr);
private:
    void onLevelNameUpdated(std::string new_name) override;
    int level_width;
    int level_height;
};

} // namespace editor