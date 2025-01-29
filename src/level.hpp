#pragma once

#include "building.hpp"
#include "effect.hpp"
#include "engine.hpp"
#include "scene.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace advanced_wars {

/**
 * @brief The main window of the game
 */
class Level : public Scene {
public:
  Level(std::string name, int width, int height, std::vector<Tile> tiles,
        std::vector<Building> buildings, std::vector<Unit> units,
        std::vector<Effect>);

  void render(Engine &engine, std::vector<SDL_Event> &events);

  int add_building(Building building);

  Building remove_building(int id);

  int add_unit(Unit unit);

  Unit remove_unit(int id);

  int add_effect(Effect effect);

  Effect remove_effect(int id);

private:
  std::string name;
  int width;
  int height;

  std::vector<Tile> tiles;
  std::unordered_map<int, Building> buildings;
  std::unordered_map<int, Unit> units;
  std::unordered_map<int, Effect> effects;

  int id;
};

} // namespace advanced_wars
