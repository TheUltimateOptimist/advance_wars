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

        void pushScene(std::shared_ptr<Scene> scene);

        std::optional<std::shared_ptr<Scene>> popScene();

        void returnToMenu();

        std::deque<SDL_Event>& events();

        void setSpritesheet(Spritesheet& spritesheet);

        Spritesheet* getSpritesheet();

        int getStage();

        void render();

        SDL_Renderer* renderer();

        ~Engine();

    private:
        Window&                             m_window;
        SDL_Renderer*                       m_SDLRenderer;
        std::vector<std::shared_ptr<Scene>> m_scenes;
        std::optional<Spritesheet*>         m_spritesheet;
        std::deque<SDL_Event>               m_events;

        bool m_quit;
        int  m_stage;
};

} // namespace advanced_wars
