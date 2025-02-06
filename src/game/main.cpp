#include "Engine.hpp"
#include "Font.hpp"
#include "Spritesheet.hpp"
#include "Window.hpp"
#include "ui/Contextmenu.hpp"
#include "ui/Menu.hpp"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <memory>
#include <stdexcept>

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

    if (TTF_Init() != 0)
    {
        throw std::runtime_error("Failed to initialize TTF: " + std::string(TTF_GetError()));
    }

    // We need this extra scope to ensure font ist dropped before TTF_Quit()
    {
        std::string fontPath =
            std::string(SDL_GetBasePath()) + std::string("res/ARCADECLASSIC.TTF");

        Font font(fontPath);

        Window window("Advanced Wars", 960, 960);

        Engine engine(window, font);

        Spritesheet spritesheet("res/spritesheet.h5", engine);

        engine.setSpritesheet(spritesheet);

        std::shared_ptr<Menu>        menu = std::make_shared<Menu>(0);
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
    }

    // Cleanup
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
