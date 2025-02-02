#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsView>
#include "MainWindow.hpp"
#include "TopBar.hpp"
#include "TileSelector.hpp"
#include "SpriteProvider.hpp"
#include <iostream>
#include "LevelView.hpp"

namespace editor 
{

MainWindow::MainWindow(LevelScene* level, QWidget *parent) : QMainWindow(parent), m_level_width(level->getWidth()), level_height(level->getHeight()) {
    //CREATE MAIN WINDOW ------------------------------------------
    QWidget *mainWidget = new QWidget(this);
    addToolBar(new TopBar(level->getName(), this));


    //CREATE TOOLBOX-----------------------------------------------
    TileSelector *tile_selector = new TileSelector(mainWidget);

    //CREATE LEVELMAP
    LevelView* level_map = new LevelView(level, this);


    //LAYOUT-------------------------------------------------------
    QHBoxLayout *layout = new QHBoxLayout(mainWidget);
    layout -> addWidget(level_map);
    layout-> addWidget(tile_selector);

    setCentralWidget(mainWidget);
    onLevelNameUpdated(level->getName());
}

void MainWindow::onLevelNameUpdated(std::string new_name)
{
    std::string dim_text = "(" + std::to_string(m_level_width) + " X " + std::to_string(level_height) + ")";
    setWindowTitle((new_name + " " + dim_text).c_str());
}

} // namespace editor