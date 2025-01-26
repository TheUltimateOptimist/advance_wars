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

  Spritesheet *spritesheet = engine.get_spritesheet();

  // Tiles
  for(Tile t : tiles)
  {
    t.render(engine, events);
  }

  // Buildings
  for (Building building : buildings) {
    SDL_Rect dst;
    dst.x = building.x * spritesheet->get_tile_width() * RENDERING_SCALE;
    dst.y = (building.y - 1) * spritesheet->get_tile_height() * RENDERING_SCALE;
    dst.w = spritesheet->get_building_width() * RENDERING_SCALE;
    dst.h = spritesheet->get_building_height() * RENDERING_SCALE;

    spritesheet->render_building(engine.renderer(), building.id,
                                 building.faction, &dst);
  }

  // Set background color for renderer
  if (SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 0)) {
    std::cout << "Could not set render draw color: " << SDL_GetError()
              << std::endl;
  }
}

} // namespace advanced_wars
