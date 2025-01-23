#pragma once

#include "SDL_render.h"
#include "scene.hpp"
#include "spritesheet.hpp"
#include "window.hpp"
#include <SDL.h>
#include <optional>
#include <vector>

namespace advanced_wars {

/**
 * @brief The main window of the game
 */
class Engine {
public:
  Engine(Window &window);

  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  bool exited();

  void pump();

  void set_scene(Scene &scene);

  void set_spritesheet(Spritesheet spritesheet);

  Spritesheet &get_spritesheet();

  void render();

  SDL_Renderer *renderer();

  ~Engine();

private:
  Window &window;
  SDL_Renderer *sdl_renderer;
  std::optional<Scene *> scene;
  std::optional<Spritesheet> spritesheet;
  std::vector<SDL_Event> events;
  bool quit;
};

} // namespace advanced_wars
