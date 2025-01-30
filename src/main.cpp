#include "engine.hpp"
#include "spritesheet.hpp"
#include "ui/contextmenu.hpp"
#include "ui/menu.hpp"
#include "window.hpp"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace advanced_wars;

int main()
{

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

    Spritesheet spritesheet("/media/data/rust/sprite-extractor/spritesheet.h5", engine);

    engine.set_spritesheet(spritesheet);

    std::shared_ptr<Menu>        menu = std::make_shared<Menu>(0);
    std::shared_ptr<ContextMenu> context_menu = std::make_shared<ContextMenu>();
    context_menu->setOptions({"Move", "Info", "Wait"});

    std::string basePath = SDL_GetBasePath();
    std::string relativePath = "assets/main_background.png";
    std::string fullPath = basePath + relativePath;
    menu->loadBackground(engine.renderer(), fullPath.c_str());

    engine.push_scene(menu);

    while (!engine.exited())
    {
        engine.pump();
        engine.render();
    }

    return 0;
}
