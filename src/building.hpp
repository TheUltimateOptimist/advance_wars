#pragma once

#include "common.h"
#include "scene.hpp"
#include "engine.hpp"

namespace advanced_wars {

enum BuildingId {
  HEADQUARTER = 0,
  CITY = 1,
  FACTORY = 2,
  PORT = 3,
  SATELLITE = 4,
};

class Building : public Scene{
public:
  Building(int x, int y, BuildingId id, Faction faction);

  int x;
  int y;
  BuildingId id;
  Faction faction;

  void render(Engine &engine, std::vector<SDL_Event> &events) override;
};

} // namespace advanced_wars