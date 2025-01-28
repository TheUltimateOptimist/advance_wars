#pragma once

#include "engine.hpp"
#include "scene.hpp"

namespace advanced_wars {

enum class BuildingFaction {
  RED = 0,
  BLUE = 1,
  YELLOW = 2,
  GREEN = 3,
  PURPLE = 4,
  NEUTRAL = 5,
};

enum class BuildingId {
  HEADQUARTER = 0,
  CITY = 1,
  FACTORY = 2,
  PORT = 3,
  SATELLITE = 4,
};

class Building {
public:
  Building(int x, int y, BuildingId id, BuildingFaction faction);

  int x;
  int y;
  BuildingId id;
  BuildingFaction faction;

  void render(Engine &engine, int scale);
};

} // namespace advanced_wars