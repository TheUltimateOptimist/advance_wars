#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "scene.hpp"
#include "tile.hpp"
#include "unit.hpp"
#include "building.hpp"
#include <string>
#include <vector>
#include <SDL.h>

namespace advanced_wars
{

/**
 * @brief The main window of the game
 */
class Level: public Scene
{
public:
    Level(std::string name, int width, int height, std::vector<Tile> tiles, std::vector<Building> buildings, std::vector<Unit> units);

    void render(SDL_Renderer *renderer, std::vector<SDL_Event> &events);

private:
    std::string name;
    int width;
    int height;
    std::vector<Building> buildings;
    std::vector<Unit> units;
};

}

#endif
