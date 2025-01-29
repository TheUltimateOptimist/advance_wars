#include "engine.hpp"
#include "spritesheet.hpp"
#include "ui/Menu.hpp"
#include "ui/ContextMenu.hpp"
#include "window.hpp"
#include <memory>

using namespace advanced_wars;

int main() {

  Window window("Advanced Wars", 800, 600);

  Engine engine(window);

  // render main menu

  std::shared_ptr<Menu> menu = std::make_shared<Menu>(0);
  std::shared_ptr<ContextMenu> context_menu = std::make_shared<ContextMenu>();
  context_menu->setOptions({"Move", "Info", "Wait"});


  std::string basePath = SDL_GetBasePath();
  std::string relativePath = "assets/main_background.png";
  std::string fullPath = basePath + relativePath;
  menu->loadBackground(engine.renderer(), fullPath.c_str());

  engine.push_scene(menu);
  /* Level level("Osnabrück", 20, 20, std::vector<Tile>(),
  std::vector<Building>(), std::vector<Unit>());

  engine.set_scene(level);

  Spritesheet spritesheet("../test.png", engine);

  engine.set_spritesheet(spritesheet); */

  while (!engine.exited()) {
    engine.pump();
    engine.render();
  }

  return 0;
}
