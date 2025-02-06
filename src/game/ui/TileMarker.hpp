/**
 * TileMarker.hpp
 *
 * @author
 */

#pragma once
#include "../Scene.hpp"

namespace advanced_wars
{
/**
 * @class TileMarker
 * @brief Renders a marker on top of a tile to mark current position
 *
 */
class TileMarker : public Scene
{
    public:
        TileMarker(int renderingScale, int tileX, int tileY, int levelWidth, int levelHeight);

        void render(Engine& engine) override;

        void handleEvent(Engine& engine, SDL_Event& event) override;

        std::pair<int, int> getPosition();

        void setPosition(int x, int y);

    private:
        int m_x;
        int m_y;
        int m_renderingScale;
        int m_width;
        int m_height;
        int m_levelHeight;
        int m_levelWidth;
};
} // namespace advanced_wars