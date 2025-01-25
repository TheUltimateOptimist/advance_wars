#pragma once

#include "common.h"

enum BuildingId {
  HEADQUARTER = 0,
  CITY = 1,
  FACTORY = 2,
  PORT = 3,
  SATELLITE = 4,
};

class Building {
public:
  Building(int x, int y, BuildingId id, Faction faction);

  int x;
  int y;
  BuildingId id;
  Faction faction;
};
