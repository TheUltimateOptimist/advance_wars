#include "building.hpp"
#include "common.h"
#include "spritesheet.hpp"

namespace advanced_wars
{

Building::Building(int x, int y, BuildingId id, Faction faction)
    : x(x), y(y), id(id), faction(faction) {};

void Building::render(Engine &engine, std::vector<SDL_Event> &events)
{
    Spritesheet* spritesheet = engine.get_spritesheet();

    SDL_Rect src;
    src.x = id * spritesheet->get_building_width();
    src.y = 0;
    src.w = spritesheet->get_building_width();
    src.h = spritesheet->get_building_height();

    SDL_Rect dst;
    dst.x = x * spritesheet->get_tile_width() * 3;
    dst.y = (y - 1) * spritesheet->get_tile_height() * 3;
    dst.w = spritesheet->get_building_width() * 3;
    dst.h = spritesheet->get_building_height() * 3;

    SDL_RenderCopyEx(engine.renderer(), spritesheet->get_building_textures()[faction], &src, &dst, 0, NULL, SDL_FLIP_NONE);
}

} // namespace advanced_wars