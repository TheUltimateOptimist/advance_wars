#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(320, 320); // Adjust size as needed
    window.show();

    return app.exec();
}