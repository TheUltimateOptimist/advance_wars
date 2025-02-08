/**
* MainWindow.cpp
*
* @date 27.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "MainWindow.hpp"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <iostream>

#include "LevelView.hpp"
#include "SpriteProvider.hpp"
#include "TileSelector.hpp"
#include "TopBar.hpp"


namespace editor
{

MainWindow::MainWindow(LevelScene* level, QWidget* parent)
    : QMainWindow(parent), m_level_width(level->getWidth()), level_height(level->getHeight())
{
    // CREATE MAIN WINDOW ------------------------------------------
    QWidget* mainWidget = new QWidget(this);
    addToolBar(new TopBar(level->getName(), this));

    // CREATE TOOLBOX-----------------------------------------------
    TileSelector* tile_selector = new TileSelector(mainWidget);

    // CREATE LEVELMAP
    LevelView* level_map = new LevelView(level, this);
    level->setParent(level_map); // allow Qt to handle the memory

    // LAYOUT-------------------------------------------------------
    QHBoxLayout* layout = new QHBoxLayout(mainWidget);
    layout->addWidget(level_map);
    layout->addWidget(tile_selector);

    setCentralWidget(mainWidget);
    onLevelNameUpdated(level->getName());
}

void MainWindow::onLevelNameUpdated(std::string new_name)
{
    std::string dim_text =
        "(" + std::to_string(m_level_width) + " X " + std::to_string(level_height) + ")";
    setWindowTitle((new_name + " " + dim_text).c_str());
}

} // namespace editor