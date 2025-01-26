#include "tile.hpp"
#include "spritesheet.hpp"
#include <stdexcept>
#include <vector>

namespace advanced_wars {

Tile::Tile(TileId id, int x, int y)
    : id(id), x(x), y(y) {

      };

void Tile::render(Engine &engine, int scale) {
  Spritesheet *spritesheet = engine.get_spritesheet();
  int max_steps = spritesheet->get_tile_steps(static_cast<int>(id));

  int step = engine.get_stage() % max_steps;

  if (step >= max_steps || step < 0) {
    throw std::runtime_error("Tried to access step " + std::to_string(step) +
                             " for tile " + std::to_string(this->id));
  }

  std::vector<std::pair<int, int>> tiles = spritesheet->get_tiles();

  int offset = tiles.at(id).first;

  SDL_Rect src;
  src.x = offset * spritesheet->get_tile_width() +
          step * spritesheet->get_tile_width();
  src.y = 0;
  src.w = spritesheet->get_tile_width();
  src.h = spritesheet->get_tile_height();

  SDL_Rect dest;
  dest.x = x * spritesheet->get_tile_width() * scale;
  dest.y = y * spritesheet->get_tile_height() * scale;
  dest.w = spritesheet->get_tile_width() * scale;
  dest.h = spritesheet->get_tile_height() * scale;

  SDL_RenderCopyEx(engine.renderer(), spritesheet->get_tile_texture(), &src,
                   &dest, 0, NULL, SDL_FLIP_NONE);
}

} // namespace advanced_wars