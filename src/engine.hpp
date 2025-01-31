#pragma once

#include "SDL_events.h"
#include "scene.hpp"
#include "spritesheet.hpp"
#include "window.hpp"
#include <SDL.h>
#include <SDL_render.h>
#include <deque>
#include <memory>
#include <optional>

namespace advanced_wars
{

// Forward declaration
class Scene;

/**
 * @brief The main window of the game
 */
class Engine
{
    public:
        Engine(Window& window);

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        bool exited();

        void exit();

        void pump();

        void push_scene(std::shared_ptr<Scene> scene);

        std::optional<std::shared_ptr<Scene>> pop_scene();

        void return_to_menu();

        std::deque<SDL_Event>& events();

        void set_spritesheet(Spritesheet& spritesheet);

        Spritesheet* get_spritesheet();

        int get_stage();

        void render();

        SDL_Renderer* renderer();

        ~Engine();

    private:
        Window&                             m_window;
        SDL_Renderer*                       m_sdl_renderer;
        std::vector<std::shared_ptr<Scene>> m_scenes;
        std::optional<Spritesheet*>         m_spritesheet;
        std::deque<SDL_Event>               m_events;
        bool                                m_quit;
        int                                 m_stage;
};

} // namespace advanced_wars
