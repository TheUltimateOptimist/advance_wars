#pragma once

#include "scene.hpp"
#include "engine.hpp"

namespace advanced_wars {

enum TileId {
  PLAIN = 0,
  WATER = 1,
  STREET_HORIZONTAL = 2,
  STREET_VERTICAL = 3,
  STREET_CORNER_TOP_LEFT = 4,
  STREET_CORNER_TOP_RIGHT = 5,
  STREET_CORNER_BOTTOM_LEFT = 6,
  STREET_CORNER_BOTTOM_RIGHT = 7,
  RIFF = 8,
  CLIFF_TOP = 9,
  CLIFF_BOTTOM = 10,
  CLIFF_LEFT = 11,
  CLIFF_RIGHT = 12,
  CLIFF_CORNER_TOP_LEFT = 13,
  CLIFF_CORNER_TOP_RIGHT = 14,
  CLIFF_CORNER_BOTTOM_LEFT = 15,
  CLIFF_CORNER_BOTTOM_RIGHT = 16,
  CLIFF_INVERSE_CORNER_TOP_LEFT = 17,
  CLIFF_INVERSE_CORNER_TOP_RIGHT = 18,
  CLIFF_INVERSE_CORNER_BOTTOM_LEFT = 19,
  CLIFF_INVERSE_CORNER_BOTTOM_RIGHT = 20,
};

class Tile : public Scene{
public:
  Tile(TileId id, int x, int y);
  TileId id;
  int x;
  int y;

  void render(Engine &engine, std::vector<SDL_Event>& events) override;
};

} // namespace advanced_wars