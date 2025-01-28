#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QGraphicsView>
#include "SpriteProvider.hpp"
#include "MainWindow.hpp"
#include "TopBar.hpp"
#include "TileSelector.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Create the main widget and layout
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    setWindowTitle("Level Editor");
    addToolBar(new TopBar("Cooles level", 200, 200, this));

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 400, 400); // Set the scene dimensions
    SpriteProvider tileprovider = SpriteProvider::from_spritesheet("../res/spritesheet.h5");
    QPixmap pixmap = tileprovider.get_sprite(0);
    QGraphicsPixmapItem* pixmapitem = scene->addPixmap(pixmap);

    // Create a view to display the scene
    QGraphicsView* view = new QGraphicsView(scene, this);
    view->scale(10, 10);
    mainLayout->addWidget(view);
    mainLayout->addWidget(new TileSelector(this));
    setCentralWidget(centralWidget);
}