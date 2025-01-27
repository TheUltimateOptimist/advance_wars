#include "building.hpp"
#include "effect.hpp"
#include "engine.hpp"
#include "level.hpp"
#include "spritesheet.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "window.hpp"
#include <cstddef>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdexcept>


using namespace advanced_wars;

int main() {

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

  Window window("Advanced Wars", 960, 960);

  Engine engine(window);

  Level level = Level::loadLevel("spritesheet.h5");

  engine.set_scene(level);

  Spritesheet spritesheet("spritesheet.h5", engine);

  engine.set_spritesheet(spritesheet);

  while (!engine.exited()) {
    engine.pump();
    engine.render();
  }

  return 0;
}
