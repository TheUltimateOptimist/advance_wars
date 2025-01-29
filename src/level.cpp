#include "level.hpp"
#include "building.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <iostream>
#include <string>
#include "ui/pausemenu.hpp"
#include "ui/ContextMenu.hpp"

namespace advanced_wars {

Level::Level(std::string name, int width, int height, std::vector<Tile> tiles,
             std::vector<Building> buildings, std::vector<Unit> units)
    : name(name), width(width), height(height), buildings(buildings),
      units(units), context_menu(ContextMenu()), context_menu_active(false){
        context_menu.setOptions({"Move", "Info", "Wait"});
      }

void Level::render(Engine *engine) {
  // Iterate over all events
  while (!engine->events().empty()) {
    handleEvent(engine, engine->events().at(0));
    engine->events().pop_front();
  }
  // Set background color for renderer
  if (SDL_SetRenderDrawColor(engine->renderer(), 0, 0, 0, 255)) {
    std::cout << "Could not set render draw color: " << SDL_GetError()
              << std::endl;
  }

   SDL_RenderClear(engine->renderer());

  if(context_menu_active) {
    context_menu.render(engine);
  }
  
}

void Level::handleEvent(Engine *engine, SDL_Event &event) {
  // Handle events for the level
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      // Pause the game
      std::cout << "Pausing game..." << std::endl;
      SDL_Texture *currentTexture = SDL_CreateTexture(engine->renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);

      PauseMenu pauseMenu(0, currentTexture);
      engine->push_scene(std::make_shared<PauseMenu>(pauseMenu));  
    }
    if(context_menu_active){
       if(event.key.keysym.sym == SDLK_DOWN) {
      context_menu.handleEvent(event);
    }
    if(event.key.keysym.sym == SDLK_UP) {
      context_menu.handleEvent(event);
    }
    if(event.key.keysym.sym == SDLK_RETURN) {
      if(context_menu.getSelectedOption() == "Wait"){
        context_menu_active = false;
      }
    }
   
    }

  }
  if(event.type == SDL_MOUSEBUTTONDOWN) {
      context_menu.update(event.button.x, event.button.y);
      context_menu_active = true;
    }
}
  
} // namespace advanced_wars
