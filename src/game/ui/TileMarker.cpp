#include "TileMarker.hpp"

namespace advanced_wars
{
TileMarker::TileMarker(int renderingScale, int tileX, int tileY)
    : m_renderingScale(renderingScale), m_width(16), m_height(16)
{
    updatePosition(tileX, tileY);
}

void TileMarker::updatePosition(int TileX, int TileY)
{
    int tileSize = 16 * m_renderingScale;
    m_x = TileX * tileSize;
    m_y = TileY * tileSize + (tileSize - m_height);
}

void TileMarker::render(Engine& engine)
{
    SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 255);
    SDL_Rect box = {m_x, m_y, m_width, m_height};
    SDL_RenderFillRect(engine.renderer(), &box);
}

void TileMarker::handleEvent(Engine& engine, SDL_Event& event) {}

} // namespace advanced_wars
