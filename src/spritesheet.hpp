#pragma once

#include "common.h"
#include <SDL.h>
#include <SDL_render.h>
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

  std::vector<std::pair<int, int>> get_tiles();

  SDL_Texture* get_tile_texture();

  // Buildings: TODO
  int get_building_width();

  int get_building_height();

  std::vector<SDL_Texture*> get_building_textures();

  // Units: TODO

private:
  // Tiles
  SDL_Texture *tile_texture;
  int tile_width;
  int tile_height;
  std::vector<std::pair<int, int>> tiles;

  // Buildings
  std::vector<SDL_Texture *> building_textures;
  int building_width;
  int building_height;
};
} // namespace advanced_wars
