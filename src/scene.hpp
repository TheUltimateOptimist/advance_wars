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
        virtual void render(Engine* engine) = 0;
};

} // namespace advanced_wars
