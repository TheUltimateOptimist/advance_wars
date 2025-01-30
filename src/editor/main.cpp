#include <QApplication>
#include "MainWindow.hpp"
#include "SpriteProvider.hpp"
#include "LevelScene.hpp"

int main(int argc, char *argv[]) {
    //argc 1 ist der Name des Programms
    if (argc < 2) {         //keine Argumente übergeben
        std::cerr << "Bitte uebergben Sie den Pfad zu dem Level, das sie bearbeiten wollen oder die Breite und Hoehe des Levels, das sie neu erstellen wollen." << std::endl;
        return 1;
    }
    if (argc > 3){          //mehr als 2 Argumente übergeben
        std::cerr << "Zuviele Kommandozeilenargumente." << std::endl;
        return 1;
    }

    //Programm beginnt
    QApplication app(argc, argv);
    SpriteProvider::initialize("../res/spritesheet.h5");
    LevelScene* level;

    if (argc == 2) {        //1 Argument übergeben: Datei Pfad
        level = LevelScene::fromFile(argv[1]);
    } else {                //2 Argumente übergeben: Breite, Höhe
        level = LevelScene::empty("Mein Level", std::stoi(argv[1]), std::stoi(argv[2]));
    }
    
    MainWindow window(level);
    window.resize(1300, 800);
    window.show();
    return app.exec();
}