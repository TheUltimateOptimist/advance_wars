#include "Tile.hpp"
#include "Spritesheet.hpp"
#include <vector>

namespace advanced_wars
{

Tile::Tile(TileId id, int x, int y) : m_id(id), m_x(x), m_y(y) {}

void Tile::render(Engine& engine, int scale, int offset_x, int offset_y)
{
    Spritesheet* spritesheet = engine.getSpritesheet();

    int step = engine.getStage() % spritesheet->getTileTextures().at(static_cast<int>(m_id)).second;

    SDL_Rect src;
    src.x = step * spritesheet->getTileWidth();
    src.y = 0;
    src.w = spritesheet->getTileWidth();
    src.h = spritesheet->getTileHeight();

    SDL_Rect dest;
    dest.x = m_x * spritesheet->getTileWidth() * scale + offset_x;
    dest.y = m_y * spritesheet->getTileHeight() * scale + offset_y;
    dest.w = spritesheet->getTileWidth() * scale;
    dest.h = spritesheet->getTileHeight() * scale;

    SDL_RenderCopyEx(
        engine.renderer(), spritesheet->getTileTextures().at(static_cast<int>(m_id)).first, &src,
        &dest, 0, NULL, SDL_FLIP_NONE);
}

} // namespace advanced_wars