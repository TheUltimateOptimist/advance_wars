#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "window.hpp"
#include "scene.hpp"
#include <SDL.h>
#include <vector>

namespace advanced_wars
{

/**
 * @brief The main window of the game
 */
class Engine
{
public:

    Engine(Window &window, Scene &scene);

    /**
     * Forbids the creation of copies of a window
     */
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    bool exited();

    void pump();

    void set_scene();

    void render();

private:
    Window &window;
    Scene &scene;
    SDL_Renderer* renderer;
    std::vector<SDL_Event> events;
    bool quit;
};

}

#endif
