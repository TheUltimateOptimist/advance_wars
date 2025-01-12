#include "scene.hpp"
#include "window.hpp"
#include "engine.hpp"
#include <vector>
#include <stdexcept>
#include <SDL.h>

namespace advanced_wars {

Engine::Engine(Window &window, Scene &scene): window(window), scene(scene) {
  this->renderer = SDL_CreateRenderer(
      this->window.sdl_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (renderer == nullptr) {
    throw std::runtime_error("SDL could not generate renderer: " +
                             std::string(SDL_GetError()));
  }
}

void Engine::set_scene() {}

void Engine::pump() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            this->quit = true;
        } else {
            this->events.push_back(e);
        }
    }
}

bool Engine::exited() {
    return this->quit;
}

void Engine::render() {
    if(SDL_RenderClear(this->renderer) != 0) {
        throw std::runtime_error("Could not clear renderer: " + std::string(SDL_GetError()));
    }

    this->scene.render(this->renderer, this->events);

    SDL_RenderPresent(this->renderer);
}

} // namespace advanced_wars
