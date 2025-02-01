#pragma once

#include "engine.hpp"
#include <SDL.h>

namespace advanced_wars
{

// Forward declaration
class Engine;

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
