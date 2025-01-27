#include "box2d/box2d.h"
#include "engine.hpp"
#include "level.hpp"
#include "spritesheet.hpp"
#include "tile.hpp"
#include "window.hpp"
#include <vector>

using namespace advanced_wars;

int main() {
  b2WorldDef worldDef = b2DefaultWorldDef();

  worldDef.gravity = (b2Vec2){0.0f, -10.0f};

  b2WorldId worldId = b2CreateWorld(&worldDef);

  Window window("Advanced Wars", 960, 960);

  Engine engine(window);

  Level level("Osnabrück", 20, 20, std::vector<Tile>(), std::vector<Building>(),
              std::vector<Unit>());

  engine.set_scene(level);

  Spritesheet spritesheet("../tiles.png", engine);

  engine.set_spritesheet(spritesheet);

  while (!engine.exited()) {
    engine.pump();
    engine.render();
  }

  return 0;
}
