#include "engine.hpp"
#include "level.hpp"
#include "spritesheet.hpp"
#include "window.hpp"
#include <vector>
#include "ui/Menu.hpp"

using namespace advanced_wars;

int main() {

  Window window("Advanced Wars", 800, 600);

  Engine engine(window);

  // render main menu

  Menu menu(0);

  std::string basePath = SDL_GetBasePath();
  std::string relativePath = "assets/main_background.png";
  std::string fullPath = basePath + relativePath;
  menu.loadBackground(engine.renderer(), fullPath.c_str());

  engine.set_scene(menu);
  /* Level level("Osnabrück", 20, 20, std::vector<Tile>(), std::vector<Building>(),
              std::vector<Unit>());

  engine.set_scene(level);

  Spritesheet spritesheet("../test.png", engine);

  engine.set_spritesheet(spritesheet); */

  while (!engine.exited()) {
    engine.pump();
    engine.render();
  }

  return 0;
}
