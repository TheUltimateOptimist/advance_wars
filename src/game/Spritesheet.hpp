/**
 * Spritesheet.hpp
 *
 * @date 30.1.2025
 * @author Frederik Keens
 * @author David Maul
 */

#pragma once

#include <SDL.h>
#include <SDL_render.h>
#include <string>
#include <vector>

namespace advanced_wars
{

// Forward declaration
class Engine;

/**
 * Spritesheet representation
 */
class Spritesheet
{
    public:
        /**
         * Constructor
         *
         * @param path Path to the file to load the spritesheet from
         * @param path Engine object with valid SDL context
         */
        Spritesheet(std::string path, Engine& engine);

        /**
         * Destructor
         */
        ~Spritesheet();

        Spritesheet(const Spritesheet&) = delete;

        Spritesheet& operator=(const Spritesheet&) = delete;

        // Tiles

        /**
         * @return The width of a floor tile in pixels
         */
        int getTileWidth();

        /**
         * @return The height of a floor tile in pixels
         */
        int getTileHeight();

        /**
         * Gets vector containing the pairs of an SDL Texture
         * and the number animations steps it has.
         *
         * E.g. The vector at 1 contains a pair of an SDL Texture for water, its animations
         * and the number of animation steps. Animation frames are store linearised.
         *
         * Which index respresents which tile can be found in the enum TileId of tile.hpp
         *
         * @return A vector of all floor tile textures and their animations
         */
        std::vector<std::pair<SDL_Texture*, int>>& getTileTextures();

        // Buildings

        /**
         * @return The width of a building in pixels
         */
        int getBuildingWidth();

        /**
         * @return The height of a building in pixels
         */
        int getBuildingHeight();

        /**
         * Every element represents the texture for all buildings from a faction linearised.
         *
         * Which FactionId represents which color
         * can be found in the BuildingId enum in building.hpp
         *
         * Order of the building sprites is the same as in buildingId enum in building.hpp
         *
         * @return Vector of all Building textures
         */
        std::vector<SDL_Texture*>& getBuildingTextures();

        // Units

        /**
         * @return The width of a unit while standing still in pixels
         */
        int getUnitWidth();

        /**
         * @return The height of a unit while standing still in pixels
         */
        int getUnitHeight();

        /**
         * @return The width of a unit while moving in pixels
         */
        int getUnitMovingWidth();

        /**
         * @return The height of a unit while moving in pixels
         */
        int getUnitMovingHeight();

        /**
         * Gets the hierarchical vector of all unit textures.
         *
         * The vector groups the faction, the UnitId and the state for each unit.
         * The pair consist of the texture for that combination and the number of animation steps.
         *
         * E.g. A red faction(0) Recon(2) Unit that's unavailable(1) would be at [0][2][1].
         * Animation frames are linearised.
         *
         * Indices can be found in the enums of unit.hpp
         *
         * @return A 3-dimensional vector of pairs consisting of a texture and
         * the number of animation steps
         */
        std::vector<std::vector<std::vector<std::pair<SDL_Texture*, int>>>>& getUnitTextures();

        // Effects

        /**
         * @return The width of an effect in pixels
         */
        int getEffectWidth();

        /**
         * @return The height of an effect in pixels
         */
        int getEffectHeight();

        /**
         * Vector that contains pairs of effect textures and the number of animation steps it has.
         *
         * Each texture of an element is a pair of an effect
         * and all its animation frames linearised.
         *
         * Which index represents which effect can be found in the EffectId enum in effect.hpp
         *
         * @return A vector of all effects and its animations
         */
        std::vector<std::pair<SDL_Texture*, int>>& getEffectTextures();

    private:
        // Tiles
        std::vector<std::pair<SDL_Texture*, int>> m_tileTextures;

        int m_tileWidth;
        int m_tileHeight;

        // Buildings
        std::vector<SDL_Texture*> m_buildingTextures;

        int m_buildingWidth;
        int m_buildingHeight;

        // Units
        std::vector<std::vector<std::vector<std::pair<SDL_Texture*, int>>>> m_unitTextures;

        int m_unitWidth;
        int m_unitHeight;
        int m_unitMovingWidth;
        int m_unitMovingHeight;

        // Effects
        std::vector<std::pair<SDL_Texture*, int>> m_effectTextures;

        int m_effectWidth;
        int m_effectHeight;
};
} // namespace advanced_wars
