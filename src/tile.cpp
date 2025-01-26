#include "tile.hpp"
#include "spritesheet.hpp"
#include <vector>

namespace advanced_wars {

Tile::Tile(TileId id, int x, int y)
    : id(id), x(x), y(y) {

      };

void Tile::render(Engine &engine, int scale) {
  Spritesheet *spritesheet = engine.get_spritesheet();

  int step = engine.get_stage() %
             spritesheet->get_tile_textures().at(static_cast<int>(id)).second;

  SDL_Rect src;
  src.x = step * spritesheet->get_tile_width();
  src.y = 0;
  src.w = spritesheet->get_tile_width();
  src.h = spritesheet->get_tile_height();

  SDL_Rect dest;
  dest.x = x * spritesheet->get_tile_width() * scale;
  dest.y = y * spritesheet->get_tile_height() * scale;
  dest.w = spritesheet->get_tile_width() * scale;
  dest.h = spritesheet->get_tile_height() * scale;

  SDL_RenderCopyEx(
      engine.renderer(),
      spritesheet->get_tile_textures().at(static_cast<int>(id)).first, &src,
      &dest, 0, NULL, SDL_FLIP_NONE);
}

} // namespace advanced_wars