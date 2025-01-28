#include <QApplication>
#include "MainWindow.hpp"
#include "SpriteProvider.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SpriteProvider::initialize("../res/spritesheet.h5");
    MainWindow window;
    window.show();

    return app.exec();
}