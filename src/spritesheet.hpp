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
        int get_tile_width();

        /**
         * @return The height of a floor tile in pixels
         */
        int get_tile_height();

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
        std::vector<std::pair<SDL_Texture*, int>>& get_tile_textures();

        // Buildings

        /**
         * @return The width of a building in pixels
         */
        int get_building_width();

        /**
         * @return The height of a building in pixels
         */
        int get_building_height();

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
        std::vector<SDL_Texture*>& get_building_textures();

        // Units

        /**
         * @return The width of a unit while standing still in pixels
         */
        int get_unit_width();

        /**
         * @return The height of a unit while standing still in pixels
         */
        int get_unit_height();

        /**
         * @return The width of a unit while moving in pixels
         */
        int get_unit_moving_width();

        /**
         * @return The height of a unit while moving in pixels
         */
        int get_unit_moving_height();

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
        std::vector<std::vector<std::vector<std::pair<SDL_Texture*, int>>>>& get_unit_textures();

        // Effects

        /**
         * @return The width of an effect in pixels
         */
        int get_effect_width();

        /**
         * @return The height of an effect in pixels
         */
        int get_effect_height();

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
        std::vector<std::pair<SDL_Texture*, int>>& get_effect_textures();

    private:
        // Tiles
        int                                       tile_width;
        int                                       tile_height;
        std::vector<std::pair<SDL_Texture*, int>> tile_textures;

        // Buildings
        std::vector<SDL_Texture*> building_textures;
        int                       building_width;
        int                       building_height;

        // Units
        std::vector<std::vector<std::vector<std::pair<SDL_Texture*, int>>>> unit_textures;
        int                                                                 unit_width;
        int                                                                 unit_height;
        int                                                                 unit_moving_width;
        int                                                                 unit_moving_height;

        // Effects
        std::vector<std::pair<SDL_Texture*, int>> effect_textures;
        int                                       effect_width;
        int                                       effect_height;
};
} // namespace advanced_wars
