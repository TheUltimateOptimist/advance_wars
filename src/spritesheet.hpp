#pragma once

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

  SDL_Texture *get_tile_texture();

  // Buildings
  int get_building_width();

  int get_building_height();

  std::vector<SDL_Texture *> &get_building_textures();

  // Units
  int get_unit_width();

  int get_unit_height();

  int get_unit_moving_width();

  int get_unit_moving_height();

  std::vector<std::vector<std::vector<std::pair<SDL_Texture *, int>>>> &
  get_unit_textures();

  // Effects
  int get_effect_width();

  int get_effect_height();

  std::vector<std::pair<SDL_Texture *, int>> &get_effect_textures();

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

  // Units
  std::vector<std::vector<std::vector<std::pair<SDL_Texture *, int>>>>
      unit_textures;
  int unit_width;
  int unit_height;
  int unit_moving_width;
  int unit_moving_height;

  // Effects
  std::vector<std::pair<SDL_Texture *, int>> effect_textures;
  int effect_width;
  int effect_height;
};
} // namespace advanced_wars
