#include "level.hpp"
#include "SDL_error.h"
#include "building.hpp"
#include "effect.hpp"
#include "engine.hpp"
#include "spritesheet.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <iostream>
#include <string>

namespace advanced_wars {

Level::Level(std::string name, int width, int height, std::vector<Tile> tiles,
             std::vector<Building> buildings, std::vector<Unit> units,
             std::vector<Effect> effects)
    : name(name), width(width), height(height), tiles(tiles),
      buildings(buildings), units(units), effects(effects) {

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

  

  // Tiles
  for (Tile &tile : tiles) {
    tile.render(engine, RENDERING_SCALE);
  }

  // Buildings
  for (Building &building : buildings) {
    building.render(engine, RENDERING_SCALE);
  }

  // Units
  for (Unit &unit : units) {
    unit.render(engine, RENDERING_SCALE);
  }

  // Effects
  for (Effect &effect : effects) {
    effect.render(engine, RENDERING_SCALE);
  }

  // Set background color for renderer
  if (SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 0)) {
    std::cout << "Could not set render draw color: " << SDL_GetError()
              << std::endl;
  }
}

} // namespace advanced_wars
