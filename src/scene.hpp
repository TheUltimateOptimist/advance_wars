#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL.h>
#include <vector>

namespace advanced_wars {

class Scene {
public:
virtual void render(SDL_Renderer *renderer, std::vector<SDL_Event> &events) = 0;
};
}

#endif
