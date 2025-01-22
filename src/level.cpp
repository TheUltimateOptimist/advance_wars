#include "level.hpp"
#include "building.hpp"
#include "unit.hpp"
#include <string>
#include <iostream>
#include <SDL2/SDL.h>

namespace advanced_wars
{

    Level::Level(std::string name, int width, int height, std::vector<Tile> tiles, std::vector<Building> buildings, std::vector<Unit> units): name(name), width(width), height(height), buildings(buildings), units(units) {};

    void Level::render(SDL_Renderer *renderer, std::vector<SDL_Event> &events) {
        // Iterate over all events
        while (!events.empty()) {
            events.erase(events.begin());
        }

        // Set background color for renderer
        if(SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0))
        {
            std::cout << "Could not set render draw color: " << SDL_GetError() << std::endl;
        }
    }

}
