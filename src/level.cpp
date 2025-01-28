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
    : name(name), width(width), height(height), tiles(tiles), buildings(),
      units(), effects() {

  for (Building building : buildings) {
    this->add_building(building);
  }

  for (Unit unit : units) {
    this->add_unit(unit);
  }

  for (Effect effect : effects) {
    this->add_effect(effect);
  }

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
  for (auto &[id, building] : buildings) {
    building.render(engine, RENDERING_SCALE);
  }

  // Units
  for (auto &[id, unit] : units) {
    unit.render(engine, RENDERING_SCALE);
  }

  // Effects
  std::vector<int> effects_to_remove;
  for (auto &[id, effect] : effects) {
    if (effect.is_finished(engine)) {
      effects_to_remove.push_back(id);
    } else {
      effect.render(engine, RENDERING_SCALE);
    }
  }

  // Remove finished effects after iteration
  for (int id : effects_to_remove) {
    this->remove_effect(id);
  }

  // Set background color for renderer
  if (SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 0)) {
    std::cout << "Could not set render draw color: " << SDL_GetError()
              << std::endl;
  }
}

int Level::add_building(Building building) {
  buildings.insert({id, building});
  id += 1;

  return id - 1;
}

Building Level::remove_building(int id) {
  Building value = buildings.at(id);
  buildings.erase(id);

  return value;
}

int Level::add_unit(Unit unit) {
  units.insert({id, unit});
  id += 1;

  return id - 1;
}

Unit Level::remove_unit(int id) {
  Unit value = units.at(id);
  units.erase(id);

  return value;
}

int Level::add_effect(Effect effect) {
  effects.insert({id, effect});
  id += 1;

  return id - 1;
}

Effect Level::remove_effect(int id) {
  Effect value = effects.at(id);
  effects.erase(id);

  return value;
}

} // namespace advanced_wars
