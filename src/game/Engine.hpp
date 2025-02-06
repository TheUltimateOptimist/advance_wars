/**
 * Engine.hpp
 *
 * @author Frederik Keens
 */

#pragma once

#include "Config.hpp"
#include "SDL_events.h"
#include "Scene.hpp"
#include "Spritesheet.hpp"
#include "Window.hpp"
#include <SDL.h>
#include <SDL_render.h>
#include <deque>
#include <memory>
#include <optional>

namespace advanced_wars
{

// Forward declaration
class Scene;
class Config;

/**
 * @brief The main window of the game
 */
class Engine
{
    public:
        /**
         * Constructor
         *
         * @param window The window to use as main window and render
         */
        Engine(Window& window);

        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;

        /**
         * Returns the state of the engine.
         *
         * @return true when running, false when exited
         */
        bool exited();

        /**
         * Shut down the engine.
         */
        void exit();

        /**
         * Handles the events of the current frame.
         *
         * Will give the event handler of the current scene every SDL event that occured.
         * If the engines scene dequeue was modified since the last pump cycle this will also change
         * the current scene.
         */
        void pump();

        /**
         * Push a scene into the scene dequeue.
         *
         * @param A shared pointer to the scene to push
         */
        void pushScene(std::shared_ptr<Scene> scene);

        /**
         * Pops a scene from the scene dequeue.
         *
         * @return The scene that got removed
         */
        std::optional<std::shared_ptr<Scene>> popScene();

        /**
         * Changes the scene to the main menu.
         */
        void returnToMenu();

        /**
         * Gets the queue of SDL events that occured.
         *
         * @return The dequeue of SDL events
         */
        std::deque<SDL_Event>& events();

        /**
         * Sets this engines Spritesheet.
         *
         * @param spritesheet The spritesheet
         */
        void setSpritesheet(Spritesheet& spritesheet);

        /**
         * Gets the current Spritesheet.
         *
         * @return The Spritesheet
         */
        Spritesheet* getSpritesheet();

        /**
         * Gets the current stage of the engine.
         *
         * @return The stage
         */
        int getStage();

        /**
         * Gets the unit config.
         *
         * @return The config
         */
        Config& getUnitConfig();

        /**
         * Render the current scene.
         */
        void render();

        /**
         * Gets the rendering context of the engine.Building
         *
         * @return The SDL renderer
         */
        SDL_Renderer* renderer();

        ~Engine();

    private:
        Window&                             m_window;      // The main window
        SDL_Renderer*                       m_SDLRenderer; // The rendering context
        std::vector<std::shared_ptr<Scene>> m_scenes;      // The scene dequeue
        std::optional<Spritesheet*>         m_spritesheet; // The current Spritesheet
        std::deque<SDL_Event> m_events; // The SDL events that occured since the last pump

        bool m_quit;  // Flag for quitting the engine
        int  m_stage; // The stage for animation purposes

        Config m_unitConfig; // Config with unit stats
};

} // namespace advanced_wars
