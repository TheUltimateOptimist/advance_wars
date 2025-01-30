#include "level.hpp"
#include "SDL_error.h"
#include "building.hpp"
#include "effect.hpp"
#include "engine.hpp"
#include "spritesheet.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <iostream>
#include <string>
#include <algorithm> 

namespace advanced_wars {

Level::Level(std::string name, int width, int height, std::vector<Tile> tiles,
             std::vector<Building> buildings, std::vector<Unit> units,
             std::vector<Effect> effects)
    : name(name), width(width), height(height), tiles(tiles),
      buildings(buildings), units(units), effects(effects) {

  if ((size_t)(width * height) != tiles.size()) {
    throw std::runtime_error("level tile mismatch");
  }
};

const int RENDERING_SCALE = 3;

bool Level::click_check_left(int tileX, int tileY) {
  
  if(selectUnit(tileX, tileY)) {
    return true;
  }

  if(selectBuilding(tileX, tileY)) {
    return true; 
  } 
    
  return false;
}

bool Level::click_check_right(int tileX, int tileY) {

  if(target_unit(tileX, tileY)) {
    return true;
  }
   
  return false;
}

bool Level::selectUnit (int tileX, int tileY) {

  //std::cout << "tileX:" << tileX << "tileX:" << tileY << std::endl;
  for (auto& unit : units) {

    if(unit.x == tileX && unit.y == tileY) {
      //std::cout << "unitX:" << unit.x << "unitY:" << unit.y << std::endl;
      
      selectedUnit = &unit;
      return true;
    }
  }
  
  return false;
}

bool Level::target_unit (int tileX, int tileY) {

  //std::cout << "tileX:" << tileX << "tileX:" << tileY << std::endl;
  for (auto& unit : units) {

    if(unit.x == tileX && unit.y == tileY) {
      //std::cout << "unitX:" << unit.x << "unitY:" << unit.y << std::endl;
      
      targetedUnit = &unit;
      return true;
    }
  }
  
  return false;
}

bool Level::selectBuilding (int tileX, int tileY) {

  for (auto& building : buildings) {

    if(building.x == tileX && building.y == tileY) {
      //std::cout << "X:" << unit.x << "Y:" << unit.y << std::endl;
      selectedBuilding = &building;
      return true;
    }
  }
  return false;
}

void Level::handleEvent(Engine &engine, SDL_Event &event) {

  switch (event.type)
  {
  case SDL_MOUSEBUTTONDOWN:

      if (event.button.button == SDL_BUTTON_LEFT) {

        int tileX = event.button.x/(16*RENDERING_SCALE);
        int tileY = event.button.y/(16*RENDERING_SCALE);

        if(click_check_left(tileX,tileY)) {
        
          if(selectedUnit) {
            selectedUnit->on_left_click(event, units);
          }

          if(selectedBuilding) {
            //building stuff
          }

        } else {

          std::cout << "Neither building nor unit clicked!" << std::endl;
          selectedUnit = nullptr;
          selectedBuilding = nullptr;

      }

      } else if (event.button.button == SDL_BUTTON_RIGHT) {

        if(selectedUnit) {

          int tileX = event.button.x / (16*RENDERING_SCALE);
          int tileY = event.button.y / (16*RENDERING_SCALE);

          if(click_check_right(tileX, tileY)) {

            selectedUnit->attack(targetedUnit);

            units.erase(
            std::remove_if(units.begin(), units.end(),
                       [](const Unit& unit) { return unit.health <= 0; }),
            units.end());

          } else {
            
            selectedUnit->update_position(tileX, tileY);

          }
        } else {

          std::cout << "No unit selected! " << std::endl;

        }
      }
    }
  }

void Level::render(Engine &engine, std::vector<SDL_Event> &events) {
  
  // Iterate over all events
  while (!events.empty()) {
    //events.erase(events.begin());

    handleEvent(engine, events.at(0));
    events.erase(events.begin());
  }

  // Tiles
  for (Tile &tile : tiles) {
    tile.render(engine, RENDERING_SCALE);
  }

  // Buildings
  for (Building &building : buildings) {
    building.render(engine, RENDERING_SCALE);
  }

  // Units
  for (Unit &unit : units) {
    unit.render(engine, RENDERING_SCALE);
  }

  // Effects
  for (Effect &effect : effects) {
    effect.render(engine, RENDERING_SCALE);
  }

  // Set background color for renderer
  if (SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 0)) {
    std::cout << "Could not set render draw color: " << SDL_GetError()
              << std::endl;
  }
}

} // namespace advanced_wars
