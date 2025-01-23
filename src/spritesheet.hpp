#pragma once

#include "SDL_render.h"
#include <SDL.h>
#include <string>
#include <vector>

namespace advanced_wars {

// Forward declaration
class Engine;

class Spritesheet {
public:
  Spritesheet(std::string path, Engine &engine);

  ~Spritesheet();

  Spritesheet(const Spritesheet &) = delete;

  Spritesheet &operator=(const Spritesheet &) = delete;

  // Tiles
  int get_tile_steps(int tile);

  int get_tile_width();

  int get_tile_height();

  int render_tile(SDL_Renderer *renderer, int tile, int step, SDL_Rect *rect);

  // Units: TODO

  // Buildings: TODO

private:
  SDL_Texture *texture;
  // Tiles
  int tile_width;
  int tile_height;
  std::vector<std::pair<int, int>> tiles;
};
} // namespace advanced_wars
