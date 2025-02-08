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
        /**
         * Constructor
         *
         * Creates a Marker at the given postion.
         * Needs the boundaries of the level to not step out of them.
         *
         * @param renderingScale The rendering scale for the current game
         * @param tileX The tile-based x-postion where the Marker should start
         * @param tileY The tile-based y-position where the Marker should start
         * @param levelWidth The width of the level
         * @param levelHeight The height of the level
         */
        TileMarker(int renderingScale, int tileX, int tileY, int levelWidth, int levelHeight);

        /**
         * Render this object
         *
         * @param engine An engine object with valid rendering context
         */
        void render(Engine& engine) override;

        /**
         * Event Handler for the tile marker
         *
         * @param engine An engine object with valid rendering context
         * @param event The SDL event to handle
         */
        void handleEvent(Engine& engine, SDL_Event& event) override;

        /**
         * Position getter
         *
         * @return The current position as pair
         */
        std::pair<int, int> getPosition();

        /**
         * Position setter
         *
         * @param x new x-position
         * @param y new y-position
         */
        void setPosition(int x, int y);

    private:
        int m_x;              // x-position
        int m_y;              // y-position
        int m_renderingScale; // rendering scale, has to be same as the level
        int m_width;          // width of the marker (normally same as tile)
        int m_height;         // height of the marker (normally same as tile)
        int m_levelHeight;    // height of the overlying level
        int m_levelWidth;     // width of the overlying level
};
} // namespace advanced_wars