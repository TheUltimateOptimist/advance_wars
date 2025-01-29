#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsView>
#include "MainWindow.hpp"
#include "TopBar.hpp"
#include "TileSelector.hpp"
#include "LevelScene.hpp"
#include <iostream>

MainWindow::MainWindow(QGraphicsScene* level, QWidget *parent) : QMainWindow(parent) {
    //CREATE MAIN WINDOW ------------------------------------------
    QWidget *mainWidget = new QWidget(this);
    setWindowTitle("Level Editor");
    addToolBar(new TopBar("Geiles Level", 20, 20, this));


    //CREATE TOOLBOX-----------------------------------------------
    TileSelector *tile_selector = new TileSelector(mainWidget);

    //CREATE LEVELMAP
    QGraphicsView* mapWidget = new QGraphicsView(this);
    level->setParent(mapWidget);
    mapWidget->setScene(level);
    mapWidget->setAlignment(Qt::AlignCenter);
    mapWidget->scale(2, 2);

    //LAYOUT-------------------------------------------------------
    QHBoxLayout *layout = new QHBoxLayout(mainWidget);
    layout -> addWidget(mapWidget);
    layout-> addWidget(tile_selector);

    setCentralWidget(mainWidget);
}