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
        TileMarker(int renderingScale, int tileX, int tileY);

        void render(Engine& engine) override;

        void handleEvent(Engine& engine, SDL_Event& event) override;

        void updatePosition(int TileX, int TileY);

    private:
        int m_x;
        int m_y;
        int m_renderingScale;
        int m_width;
        int m_height;
};
} // namespace advanced_wars