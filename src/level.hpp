#pragma once

#include "building.hpp"
#include "scene.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <string>
#include <vector>
#include "ui/ContextMenu.hpp"

namespace advanced_wars {

/**
 * @brief The main window of the game
 */
class Level : public Scene {
public:
  Level(std::string name, int width, int height, std::vector<Tile> tiles,
        std::vector<Building> buildings, std::vector<Unit> units);

  void render(Engine *events);
  void handleEvent(Engine *engine, SDL_Event &event);

private:
  std::string name;
  int width;
  int height;
  std::vector<Building> buildings;
  std::vector<Unit> units;
  ContextMenu context_menu;
  bool context_menu_active;
};

} // namespace advanced_wars
