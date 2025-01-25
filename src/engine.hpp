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

namespace advanced_wars {

// Forward declaration
class Scene;

/**
 * @brief The main window of the game
 */
class Engine {
public:
  Engine(Window &window);

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  bool exited();

  void exit();

  void pump();

  void push_scene(std::shared_ptr<Scene> scene);

  std::optional<std::shared_ptr<Scene>> pop_scene();

  std::deque<SDL_Event> &events();

  void set_spritesheet(Spritesheet spritesheet);

  void render();

  SDL_Renderer *renderer();

  ~Engine();

private:
  Window &window;
  SDL_Renderer *sdl_renderer;
  std::vector<std::shared_ptr<Scene>> scenes;
  std::optional<Spritesheet> spritesheet;
  std::deque<SDL_Event> _events;
  bool quit;
};

} // namespace advanced_wars
