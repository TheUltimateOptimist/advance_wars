#include "level.hpp"
#include "building.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <iostream>
#include <string>

namespace advanced_wars {

Level::Level(std::string name, int width, int height, std::vector<Tile> tiles,
             std::vector<Building> buildings, std::vector<Unit> units)
    : name(name), width(width), height(height), buildings(buildings),
      units(units) {};

void Level::render(Engine *engine) {
  // Iterate over all events
  while (!engine->events().empty()) {
    engine->events().pop_front();
  }
  // Set background color for renderer
  if (SDL_SetRenderDrawColor(engine->renderer(), 255, 0, 0, 0)) {
    std::cout << "Could not set render draw color: " << SDL_GetError()
              << std::endl;
  }
}

} // namespace advanced_wars
