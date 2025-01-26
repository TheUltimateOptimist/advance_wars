#include "engine.hpp"
#include <SDL_render.h>
#include "scene.hpp"
#include "spritesheet.hpp"
#include "window.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include <vector>

namespace advanced_wars {

Engine::Engine(Window &window) : window(window), quit(false) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error("SDL could not initialize: " +
                             std::string(SDL_GetError()));
  }

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    throw std::runtime_error(
        "SDL_image could not initialize! SDL_image Error: " +
        std::string(IMG_GetError()));
  }

  this->sdl_renderer =
      SDL_CreateRenderer(this->window.sdl_window(), -1,
                         SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (sdl_renderer == nullptr) {
    throw std::runtime_error("SDL could not generate renderer: " +
                             std::string(SDL_GetError()));
  }
}

void Engine::set_scene(Scene &scene) { this->scene = &scene; }

void Engine::set_spritesheet(Spritesheet &spritesheet) {
  this->spritesheet = &spritesheet;
}

Spritesheet *Engine::get_spritesheet() { return spritesheet.value(); }

void Engine::pump() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      this->quit = true;
    } else {
      this->events.push_back(e);
    }
  }
}

bool Engine::exited() { return this->quit; }

int Engine::get_stage() { return this->stage; }

void Engine::render() {
  if (SDL_RenderClear(this->sdl_renderer) != 0) {
    throw std::runtime_error("Could not clear renderer: " +
                             std::string(SDL_GetError()));
  }

  if (!scene.has_value()) {
    return;
  }

  stage = SDL_GetTicks() / 300;

  this->scene.value()->render(*this, this->events);

  SDL_RenderPresent(this->sdl_renderer);
}

SDL_Renderer *Engine::renderer() { return this->sdl_renderer; }

Engine::~Engine() {
  SDL_DestroyRenderer(sdl_renderer);
  IMG_Quit();
  SDL_Quit();
}

} // namespace advanced_wars
