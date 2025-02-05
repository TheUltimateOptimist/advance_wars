/**
* main.cpp
*
* @date 27.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include <QApplication>

#include "SpriteProvider.hpp"
#include "MainWindow.hpp"
#include "LevelScene.hpp"

using namespace editor;

/**
 * The main function starts the program.
 * It checks if any command line arguments were provided.
 * If not it terminates. 
 * Otherwise if a path to a hdf5 level file was provided it 
 * starts the level editor and loads the level from the file for editing.
 * If a width and height was provided, it create a blank level with the given dimensions.
 */

int main(int argc, char* argv[])
{
    if (argc < 2)
    { // no arguments provided
        std::cerr << "Bitte uebergben Sie den Pfad zu dem Level, das sie bearbeiten wollen oder "
                     "die Breite und Hoehe des Levels, das sie neu erstellen wollen."
                  << std::endl;
        return 1;
    }
    if (argc > 3)
    { // more than 2 arguments provided
        std::cerr << "Zuviele Kommandozeilenargumente." << std::endl;
        return 1;
    }

    // programm starts
    QApplication app(argc, argv);

    // SpriteProvider is initialized once and can then be used from anywhere in the application
    // to get the QGraphicsPixmap for a specific tile.
    SpriteProvider::initialize("../res/spritesheet.h5");


    LevelScene* level;
    if (argc == 2)
    { // 1 argument provided => create Level from file
        level = LevelScene::fromFile(argv[1]);
    }
    else
    { // 2 arguments provided => create blank level with given width and height
        level = LevelScene::empty("Mein Level", std::stoi(argv[1]), std::stoi(argv[2]));
    }

    MainWindow window(level);
    window.resize(1300, 800);
    window.show();
    return app.exec();
}