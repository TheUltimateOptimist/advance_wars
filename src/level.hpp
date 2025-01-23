#pragma once

#include "building.hpp"
#include "engine.hpp"
#include "scene.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <string>
#include <vector>

namespace advanced_wars {

/**
 * @brief The main window of the game
 */
class Level : public Scene {
public:
  Level(std::string name, int width, int height, std::vector<Tile> tiles,
        std::vector<Building> buildings, std::vector<Unit> units);

  void render(Engine &engine, std::vector<SDL_Event> &events);

private:
  std::string name;
  int width;
  int height;
  std::vector<Building> buildings;
  std::vector<Unit> units;
};

} // namespace advanced_wars
