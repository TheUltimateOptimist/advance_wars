/**
 * Scene.hpp
 *
 * @author
 */

#pragma once

#include "Engine.hpp"
#include <SDL.h>

namespace advanced_wars
{

// Forward declaration
class Engine;

/**
 * @brief Scene Interface
 *
 * Everything, that can to be saved in the engines scene stack needs to be renderable and provide
 * a function to handle SDL events in the pump step.
 */
class Scene
{
    public:
        virtual void render(Engine& engine) = 0;

        /**
         * Every Scene has an event handler that can get called in the pump function of engine.
         *
         * It should implement handling of every event that is relevant to this scene only.
         */
        virtual void handleEvent(Engine& engine, SDL_Event& event) = 0;
};

} // namespace advanced_wars
