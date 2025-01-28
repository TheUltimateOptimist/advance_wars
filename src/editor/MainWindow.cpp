#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsView>
#include "SpriteProvider.hpp"
#include "MainWindow.hpp"
#include "TopBar.hpp"
#include "TileSelector.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    //CREATE MAIN WINDOW ------------------------------------------
    QWidget *mainWidget = new QWidget(this);
    setWindowTitle("Level Editor");
    addToolBar(new TopBar("Cooles level", 200, 200, this));

    //CREATE MAP---------------------------------------------------
    QGraphicsScene *scene = new QGraphicsScene(mainWidget);
    scene->setSceneRect(0, 0, 400, 400); // Set the scene dimensions
    SpriteProvider tileprovider = SpriteProvider::from_spritesheet("../res/spritesheet.h5");
    QPixmap pixmap = tileprovider.get_sprite(0);
    QGraphicsPixmapItem* pixmapitem = scene->addPixmap(pixmap);
    // Create a view to display the scene
    QGraphicsView* mapWidget = new QGraphicsView(scene, this);
    mapWidget->scale(10, 10);

    //CREATE TOOLBOX-----------------------------------------------
    TileSelector *tile_selector = new TileSelector(mainWidget);

    //LAYOUT-------------------------------------------------------
    QHBoxLayout *layout = new QHBoxLayout(mainWidget);
    layout -> addWidget(mapWidget);
    layout-> addWidget(tile_selector);

    setCentralWidget(mainWidget);
}