#include "TileMarker.hpp"
#include <iostream>

namespace advanced_wars
{
TileMarker::TileMarker(int renderingScale, int tileX, int tileY, int levelWidth, int levelHeight)
    : m_renderingScale(renderingScale), m_width(16), m_height(16)
{
    int tileSize = 16 * renderingScale;
    m_x = tileX * tileSize;
    m_y = tileY * tileSize + (tileSize - m_height);
    m_levelWidth = levelWidth * tileSize;
    m_levelHeight = levelHeight * tileSize;
}

void TileMarker::render(Engine& engine)
{
    SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 255);
    SDL_Rect box = {m_x, m_y, m_width, m_height};
    SDL_RenderFillRect(engine.renderer(), &box);
}

void TileMarker::handleEvent(Engine& engine, SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        int newX;
        int newY;
        switch (event.key.keysym.sym)
        {
        case SDLK_UP:
            newY = m_y - 16 * m_renderingScale;
            std::cout << "New Y: " << newY << std::endl;
            if (newY <= 0)
            {
                break;
            }
            m_y = newY;
            break;
        case SDLK_DOWN:
            newY = m_y + 16 * m_renderingScale;
            std::cout << "New Y: " << newY << std::endl;

            if (newY >= m_levelHeight)

            {
                break;
            }
            m_y = newY;
            break;
        case SDLK_RIGHT:
            newX = m_x + 16 * m_renderingScale;
            std::cout << "New X: " << newX << std::endl;

            if (newX >= m_levelWidth)
            {
                break;
            }
            m_x = newX;
            break;
        case SDLK_LEFT:
            newX = m_x - 16 * m_renderingScale;
            std::cout << "New X: " << newX << std::endl;

            if (newX <= 0)
            {
                break;
            }
            m_x = newX;
            break;

        default:
            break;
        }
    }
}

std::pair<int, int> TileMarker::getPosition()
{
    int tileX = m_x / (16 * m_renderingScale);
    int tileY = m_y / (16 * m_renderingScale);
    return {tileX, tileY};
}

} // namespace advanced_wars
