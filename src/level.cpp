#include "level.hpp"
#include "SDL_error.h"
#include "building.hpp"
#include "engine.hpp"
#include "spritesheet.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <iostream>
#include <string>

namespace advanced_wars {

Level::Level(std::string name, int width, int height, std::vector<Tile> tiles,
             std::vector<Building> buildings, std::vector<Unit> units)
    : name(name), width(width), height(height), tiles(tiles),
      buildings(buildings), units(units) {

  if ((size_t)(width * height) != tiles.size()) {
    throw std::runtime_error("level tile mismatch");
  }
};

void Level::render(Engine &engine, std::vector<SDL_Event> &events) {
  const int RENDERING_SCALE = 3;

  // Iterate over all events
  while (!events.empty()) {
    events.erase(events.begin());
  }

  size_t idx = 0;

  int stage = SDL_GetTicks() / 300;

  for (int y = 0; y < this->height; y++) {
    for (int x = 0; x < this->width; x++) {
      Spritesheet *spritesheet = engine.get_spritesheet();

      SDL_Rect dst;
      dst.x = x * spritesheet->get_tile_width() * RENDERING_SCALE;
      dst.y = y * spritesheet->get_tile_height() * RENDERING_SCALE;
      dst.w = spritesheet->get_tile_width() * RENDERING_SCALE;
      dst.h = spritesheet->get_tile_height() * RENDERING_SCALE;

      int tile_id = static_cast<int>(tiles.at(idx).id);

      if (spritesheet->render_tile(engine.renderer(), tile_id,
                                   stage % spritesheet->get_tile_steps(tile_id),
                                   &dst) != 0) {
        throw std::runtime_error("error while rendering a tile: " +
                                 std::string(SDL_GetError()));
      }

      idx += 1;
    }
  }

  // Set background color for renderer
  if (SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 0)) {
    std::cout << "Could not set render draw color: " << SDL_GetError()
              << std::endl;
  }
}

} // namespace advanced_wars
