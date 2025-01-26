#pragma once

#include "engine.hpp"
#include "scene.hpp"

namespace advanced_wars {

enum BuildingFaction {
  BRED = 0,
  BBLUE = 1,
  BYELLOW = 2,
  BGREEN = 3,
  BPURPLE = 4,
  BNEUTRAL = 5,
};

enum BuildingId {
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