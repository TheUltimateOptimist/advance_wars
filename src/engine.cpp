#include "engine.hpp"
#include "SDL_events.h"
#include "scene.hpp"
#include "spritesheet.hpp"
#include "window.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <deque>
#include <memory>
#include <optional>
#include <stdexcept>

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

std::deque<SDL_Event> &Engine::events() { return this->_events; }

void Engine::push_scene(std::shared_ptr<Scene> scene) {
  this->scenes.push_back(scene);
}

std::optional<std::shared_ptr<Scene>> Engine::pop_scene() {
  if (this->scenes.empty()) {
    return std::nullopt;
  }
    std::shared_ptr<Scene> tmp = scenes.back();
    this->scenes.pop_back();

    return tmp;
}

void Engine::set_spritesheet(Spritesheet spritesheet) {
  this->spritesheet = spritesheet;
}

void Engine::pump() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      this->quit = true;
    } else {
      this->_events.push_back(e);
    }
  }
}

void Engine::exit() { this->quit = true; }

bool Engine::exited() { return this->quit; }

void Engine::render() {
  if (SDL_RenderClear(this->sdl_renderer) != 0) {
    throw std::runtime_error("Could not clear renderer: " +
                             std::string(SDL_GetError()));
  }

  if (scenes.empty()) {
    SDL_RenderPresent(this->sdl_renderer);
    return;
  }

  std::shared_ptr<Scene> currentScene = scenes.back();
  if (!currentScene) {
    SDL_RenderPresent(this->sdl_renderer);
    return;
  }

  currentScene->render(this);

  SDL_RenderPresent(this->sdl_renderer);
}

SDL_Renderer *Engine::renderer() { return this->sdl_renderer; }

Engine::~Engine() {
  SDL_DestroyRenderer(sdl_renderer);
  IMG_Quit();
  SDL_Quit();
}

} // namespace advanced_wars
