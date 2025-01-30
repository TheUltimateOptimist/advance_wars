#include <QApplication>
#include "MainWindow.hpp"
#include "SpriteProvider.hpp"
#include "LevelScene.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SpriteProvider::initialize("../res/spritesheet.h5");
    if (argc < 2) {
        std::cerr << "Bitte uebergben Sie den Pfad zu dem Level, das sie bearbeiten wollen oder die Breite und Hoehe des Levels, das sie neu erstellen wollen." << std::endl;
        return 1;
    }
    LevelScene* level;
    if (argc == 2) {
        level = LevelScene::fromFile(argv[1]);
    } else if (argc == 3) {
        level = LevelScene::empty("Mein Level", std::stoi(argv[1]), std::stoi(argv[2]));
    } else {
        std::cerr << "Zuviele Kommandozeilenargumente." << std::endl;
        return 1;
    }
    MainWindow window(level);
    window.resize(1300, 800);
    window.show();
    return app.exec();
}