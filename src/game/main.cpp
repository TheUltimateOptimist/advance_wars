#include "Engine.hpp"
#include "Spritesheet.hpp"
#include "Window.hpp"
#include "ui/Contextmenu.hpp"
#include "ui/Menu.hpp"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace advanced_wars;

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cerr << "Please provide the path to the level that you want to play as a command line "
                     "argument."
                  << std::endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error("SDL could not initialize: " + std::string(SDL_GetError()));
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        throw std::runtime_error(
            "SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
    }

    Window window("Advanced Wars", 960, 960);

    Engine engine(window);

    Spritesheet spritesheet("res/spritesheet.h5", engine);

    engine.setSpritesheet(spritesheet);

    std::shared_ptr<Menu>        menu = std::make_shared<Menu>(0, argv[1]);
    std::shared_ptr<ContextMenu> context_menu = std::make_shared<ContextMenu>();
    context_menu->setOptions({"Move", "Info", "Wait"});

    std::string basePath = SDL_GetBasePath();
    std::string relativePath = "res/main_background.png";
    std::string fullPath = basePath + relativePath;
    menu->loadBackground(engine, fullPath.c_str());

    engine.pushScene(menu);

    while (!engine.exited())
    {
        engine.pump();
        engine.render();
    }

    return 0;
}
