#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QGraphicsView>
#include "TileProvider.hpp"
#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Create the main widget and layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    setWindowTitle("Level Editor");

    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 400, 400); // Set the scene dimensions
    TileProvider tileprovider = TileProvider::from_spritesheet("spritesheet.h5");
    QPixmap pixmap = tileprovider.get_tile(0);
    QGraphicsPixmapItem* pixmapitem = scene->addPixmap(pixmap);

    // Create a view to display the scene
    QGraphicsView* view = new QGraphicsView(scene, this);
    view->setFixedSize(300, 300);
    view->scale(10, 10);
    mainLayout->addWidget(view);

    setCentralWidget(centralWidget);
}