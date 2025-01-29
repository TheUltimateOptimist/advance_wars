#include "building.hpp"
#include "effect.hpp"
#include "engine.hpp"
#include "level.hpp"
#include "spritesheet.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "window.hpp"
#include <cstddef>
#include <vector>
#include "config.hpp"
#include <iostream>

using namespace advanced_wars;

int main()
{

  Window window("Advanced Wars", 960, 960);

  Engine engine(window);
  
  Unit::readXML();

  // Construct a level
  std::vector<Tile> tiles;
  for (int y = 0; y < 20; y++)
  {
    for (int x = 0; x < 20; x++)
    {
      tiles.push_back(Tile(TileId::PLAIN, x, y));
    }
  }

  // Fill the edges with water
  for (size_t n = 0; n < 20; n++)
  {
    // Vertical
    tiles.at(n * 20) = Tile(TileId::WATER, 0, n);
    tiles.at(n * 20 + 19) = Tile(TileId::WATER, 19, n);
    // Horizontal
    tiles.at(n) = Tile(TileId::WATER, n, 0);
    tiles.at(19 * 20 + n) = Tile(TileId::WATER, n, 19);
  }

  // Make the edges cliffs
  for (size_t n = 1; n < 19; n++)
  {
    // Vertical
    tiles.at(n * 20 + 1) = Tile(TileId::CLIFF_RIGHT, 1, n);
    tiles.at(n * 20 + 18) = Tile(TileId::CLIFF_LEFT, 18, n);

    // Horizontal
    tiles.at(20 + n) = Tile(TileId::CLIFF_BOTTOM, n, 1);
    tiles.at(18 * 20 + n) = Tile(TileId::CLIFF_TOP, n, 18);
  }

  // Fix the corners
  tiles.at(20 + 1) = Tile(TileId::CLIFF_CORNER_TOP_LEFT, 1, 1);
  tiles.at(20 + 18) = Tile(TileId::CLIFF_CORNER_TOP_RIGHT, 18, 1);
  tiles.at(18 * 20 + 1) = Tile(TileId::CLIFF_CORNER_BOTTOM_LEFT, 1, 18);
  tiles.at(18 * 20 + 18) = Tile(TileId::CLIFF_CORNER_BOTTOM_RIGHT, 18, 18);

  // Buildings
  std::vector<Building> buildings;

  for (int y = 0; y < 6; y++)
  {
    for (int x = 0; x < 5; x++)
    {
      BuildingId id = static_cast<BuildingId>(x);
      BuildingFaction faction = static_cast<BuildingFaction>(y);

      buildings.push_back(Building(3 + x, 3 + 2 * y, id, faction));
    }
  }

  // Units
  std::vector<Unit> units;

  for (int y = 0; y < 19; y++)
  {
    for (int x = 0; x < 6; x++)
    {
      units.push_back(Unit(x + 9, y + 2, UnitFaction::URED,
                           static_cast<UnitId>(y), static_cast<UnitState>(x)));
    }
  }

  std::vector<Effect> effects({Effect(3, 15, EffectId::LAND_EXPLOSION, true),
                               Effect(5, 15, EffectId::AIR_EXPLOSION, true),
                               Effect(5, 18, EffectId::NAVAL_EXPLOSION, true)});

  Level level("Osnabrück", 20, 20, tiles, buildings, units, effects);

  engine.set_scene(level);

  Spritesheet spritesheet("./spritesheet.h5",
                          engine);

  engine.set_spritesheet(spritesheet);

  while (!engine.exited())
  {
    engine.pump();
    engine.render();
  }

  return 0;
}
