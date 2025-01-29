#pragma once

#include <SDL.h>
#include <vector>

namespace advanced_wars
{

// Forward declaration
class Engine;

class Scene
{
    public:
        virtual void render(Engine& engine, std::vector<SDL_Event>& events) = 0;
};
} // namespace advanced_wars
