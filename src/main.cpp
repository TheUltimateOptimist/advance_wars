#include "building.hpp"
#include "common.h"
#include "engine.hpp"
#include "level.hpp"
#include "spritesheet.hpp"
#include "tile.hpp"
#include "window.hpp"
#include <cstddef>
#include <vector>

using namespace advanced_wars;

int main() {

  Window window("Advanced Wars", 960, 960);

  Engine engine(window);

  // Construct a level
  std::vector<Tile> tiles(20 * 20, Tile(TileId::PLAIN));

  // Fill the edges with water
  for (size_t n = 0; n < 20; n++) {
    // Vertical
    tiles.at(n * 20) = Tile(TileId::WATER);
    tiles.at(n * 20 + 19) = Tile(TileId::WATER);
    // Horizontal
    tiles.at(n) = Tile(TileId::WATER);
    tiles.at(19 * 20 + n) = Tile(TileId::WATER);
  }

  // Make the edges cliffs
  for (size_t n = 1; n < 19; n++) {
    // Vertical
    tiles.at(n * 20 + 1) = Tile(TileId::CLIFF_RIGHT);
    tiles.at(n * 20 + 18) = Tile(TileId::CLIFF_LEFT);

    // Horizontal
    tiles.at(20 + n) = Tile(TileId::CLIFF_BOTTOM);
    tiles.at(18 * 20 + n) = Tile(TileId::CLIFF_TOP);
  }

  // Fix the corners
  tiles.at(20 + 1) = Tile(TileId::CLIFF_CORNER_TOP_LEFT);
  tiles.at(20 + 18) = Tile(TileId::CLIFF_CORNER_TOP_RIGHT);
  tiles.at(18 * 20 + 1) = Tile(TileId::CLIFF_CORNER_BOTTOM_LEFT);
  tiles.at(18 * 20 + 18) = Tile(TileId::CLIFF_CORNER_BOTTOM_RIGHT);

  // Buildings
  std::vector<Building> buildings;

  for (int y = 0; y < 6; y++) {
    for (int x = 0; x < 5; x++) {
      BuildingId id = static_cast<BuildingId>(x);
      Faction faction = static_cast<Faction>(y);

      buildings.push_back(Building(3 + x, 3 + 2 * y, id, faction));
    }
  }

  Level level("Osnabrück", 20, 20, tiles, buildings, std::vector<Unit>());

  engine.set_scene(level);

  Spritesheet spritesheet("/media/data/rust/sprite-extractor/spritesheet.h5",
                          engine);

  engine.set_spritesheet(spritesheet);

  while (!engine.exited()) {
    engine.pump();
    engine.render();
  }

  return 0;
}
