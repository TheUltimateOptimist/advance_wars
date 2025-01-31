/**
 * Spritesheet.hpp
 *
 * @date 30.1.2025
 * @author Frederik Keens
 * @author David Maul
 */

#include "spritesheet.hpp"
#include "engine.hpp"
#include "highfive/H5File.hpp"
#include <SDL_image.h>
#include <SDL_render.h>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace advanced_wars
{

Spritesheet::Spritesheet(std::string path, Engine& engine)
{

    HighFive::File file(path, HighFive::File::ReadOnly);

    // Tiles
    std::vector<std::string> tiles(
        {"plain",
         "water",
         "forest",
         "mountain",
         "bridge_horizontal",
         "bridge_vertical",
         "street_horizontal",
         "street_vertical",
         "street_crossing",
         "street_junction_right",
         "street_junction_left",
         "street_junction_down",
         "street_junction_up",
         "street_corner_top_left",
         "street_corner_top_right",
         "street_corner_bottom_left",
         "street_corner_bottom_right",
         "riff",
         "cliff_top",
         "cliff_bottom",
         "cliff_left",
         "cliff_right",
         "cliff_corner_top_left",
         "cliff_corner_top_right",
         "cliff_corner_bottom_left",
         "cliff_corner_bottom_right",
         "cliff_inverse_corner_top_left",
         "cliff_inverse_corner_top_right",
         "cliff_inverse_corner_bottom_left",
         "cliff_inverse_corner_bottom_right"});

    // every sub data set of tiles
    for (size_t tile_idx = 0; tile_idx < tiles.size(); tile_idx++)
    {
        HighFive::DataSet units_ds = file.getDataSet("tiles/" + tiles[tile_idx]);

        std::vector<std::vector<std::vector<uint32_t>>> tile_frames;
        units_ds.read(tile_frames);

        std::vector<uint32_t> tile_buffer(16 * 16 * tile_frames.size(), 0);

        // every animation frame
        for (size_t n = 0; n < tile_frames.size(); n++)
        {
            for (size_t y = 0; y < 16; y++)
            {
                for (size_t x = 0; x < 16; x++)
                {
                    size_t index = (y * tile_frames.size() * 16) + (n * 16 + x);

                    tile_buffer.at(index) = tile_frames.at(n).at(16 - y - 1).at(x);
                }
            }
        }

        SDL_Texture* tmp = SDL_CreateTexture(
            engine.renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
            tile_frames.size() * 16, 16);

        SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);

        if (tmp == nullptr)
        {
            throw std::runtime_error(
                "Fehler beim Erstellen der Textur für die Units: " + std::string(SDL_GetError()));
        }

        if (SDL_UpdateTexture(
                tmp, NULL, tile_buffer.data(), tile_frames.size() * 16 * sizeof(int32_t)) != 0)
        {
            throw std::runtime_error(
                "Fehler beim updaten der Textur für die Units: " + std::string(SDL_GetError()));
        }

        m_tile_textures.push_back(std::pair<SDL_Texture*, int>(tmp, tile_frames.size()));
    }

    this->m_tile_width = 16;
    this->m_tile_height = 16;

    // Buildings
    std::vector<std::string> building_factions(
        {"red", "blue", "yellow", "green", "purple", "neutral"});

    // every sub data set of buildings
    for (std::string faction : building_factions)
    {
        HighFive::DataSet buildings_ds = file.getDataSet("buildings/" + faction);

        std::vector<std::vector<std::vector<uint32_t>>> buildings_frames;

        buildings_ds.read(buildings_frames);

        std::vector<uint32_t> building_buffer(32 * 16 * buildings_frames.size(), 0);

        // every type of building
        for (size_t n = 0; n < buildings_frames.size(); n++)
        {
            for (size_t y = 0; y < 32; y++)
            {
                for (size_t x = 0; x < 16; x++)
                {
                    size_t index = (y * buildings_frames.size() * 16) + (n * 16 + x);

                    building_buffer.at(index) = buildings_frames.at(n).at(32 - y - 1).at(x);
                }
            }
        }

        SDL_Texture* tmp = SDL_CreateTexture(
            engine.renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
            buildings_frames.size() * 16, 32);

        SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);

        if (tmp == nullptr)
        {
            throw std::runtime_error(
                "Fehler beim Erstellen der Textur für die Buildings: " +
                std::string(SDL_GetError()));
        }

        if (SDL_UpdateTexture(
                tmp, NULL, building_buffer.data(),
                buildings_frames.size() * 16 * sizeof(int32_t)) != 0)
        {
            throw std::runtime_error(
                "Fehler beim updaten der Textur für die Buildings: " + std::string(SDL_GetError()));
        }

        this->m_building_textures.push_back(tmp);
    }

    this->m_building_width = 16;
    this->m_building_height = 32;

    // Units
    std::vector<std::string> unit_factions({"red", "blue", "green", "yellow", "purple"});

    std::vector<std::string> units(
        {"infantery", "mechanized_infantery", "recon", "medium_tank", "heavy_tank", "neo_tank",
         "apc", "anti_air_tank", "artillery", "rocket_artillery", "anti_air_missile_launcher",
         "fighter", "bomber", "battle_helicopter", "transport_helicopter", "battleship", "cruiser",
         "lander", "submarine"});

    std::vector<std::string> unit_states({"idle", "unavailable"});
    std::vector<std::string> unit_movement_states({"left", "right", "down", "up"});

    // every factions sub data set
    for (size_t faction_idx = 0; faction_idx < unit_factions.size(); faction_idx++)
    {
        std::string faction = unit_factions.at(faction_idx);
        // Create entry for units for in a faction
        m_unit_textures.push_back(std::vector<std::vector<std::pair<SDL_Texture*, int>>>());

        // every unit sub data set
        for (size_t unit_idx = 0; unit_idx < units.size(); unit_idx++)
        {
            std::string unit = units.at(unit_idx);

            // Create entry for states for a unit
            m_unit_textures.at(faction_idx).push_back(std::vector<std::pair<SDL_Texture*, int>>());

            // every state sub data set
            for (size_t state_idx = 0; state_idx < unit_states.size(); state_idx++)
            {
                std::string unit_state = unit_states.at(state_idx);

                HighFive::DataSet units_ds =
                    file.getDataSet("units/" + faction + "/" + unit + "/" + unit_state);

                std::vector<std::vector<std::vector<uint32_t>>> unit_frames;
                units_ds.read(unit_frames);

                std::vector<uint32_t> unit_buffer(16 * 16 * unit_frames.size(), 0);

                for (size_t n = 0; n < unit_frames.size(); n++)
                {
                    for (size_t y = 0; y < 16; y++)
                    {
                        for (size_t x = 0; x < 16; x++)
                        {
                            size_t index = (y * unit_frames.size() * 16) + (n * 16 + x);

                            unit_buffer.at(index) = unit_frames.at(n).at(16 - y - 1).at(x);
                        }
                    }
                }

                SDL_Texture* tmp = SDL_CreateTexture(
                    engine.renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
                    unit_frames.size() * 16, 16);

                SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);

                if (tmp == nullptr)
                {
                    throw std::runtime_error(
                        "Fehler beim Erstellen der Textur für die Units: " +
                        std::string(SDL_GetError()));
                }

                if (SDL_UpdateTexture(
                        tmp, NULL, unit_buffer.data(), unit_frames.size() * 16 * sizeof(int32_t)) !=
                    0)
                {
                    throw std::runtime_error(
                        "Fehler beim updaten der Textur für die Units: " +
                        std::string(SDL_GetError()));
                }

                m_unit_textures.at(faction_idx)
                    .at(unit_idx)
                    .push_back(std::pair<SDL_Texture*, int>(tmp, unit_frames.size()));
            }

            // every movement state sub data set
            for (size_t movement_state_idx = 0; movement_state_idx < unit_movement_states.size();
                 movement_state_idx++)
            {
                std::string movement_state = unit_movement_states.at(movement_state_idx);

                HighFive::DataSet units_ds = file.getDataSet(
                    "units/" + faction + "/" + unit + "/movement/" + movement_state);

                std::vector<std::vector<std::vector<uint32_t>>> unit_frames;
                units_ds.read(unit_frames);

                std::vector<uint32_t> unit_buffer(24 * 24 * unit_frames.size(), 0);

                for (size_t n = 0; n < unit_frames.size(); n++)
                {
                    for (size_t y = 0; y < 24; y++)
                    {
                        for (size_t x = 0; x < 24; x++)
                        {
                            size_t index = (y * unit_frames.size() * 24) + (n * 24 + x);

                            unit_buffer.at(index) = unit_frames.at(n).at(24 - y - 1).at(x);
                        }
                    }
                }

                SDL_Texture* tmp = SDL_CreateTexture(
                    engine.renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
                    unit_frames.size() * 24, 24);

                SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);

                if (tmp == nullptr)
                {
                    throw std::runtime_error(
                        "Fehler beim Erstellen der Textur für die Units: " +
                        std::string(SDL_GetError()));
                }

                if (SDL_UpdateTexture(
                        tmp, NULL, unit_buffer.data(), unit_frames.size() * 24 * sizeof(int32_t)) !=
                    0)
                {
                    throw std::runtime_error(
                        "Fehler beim updaten der Textur für die Units: " +
                        std::string(SDL_GetError()));
                }

                m_unit_textures.at(faction_idx)
                    .at(unit_idx)
                    .push_back(std::pair<SDL_Texture*, int>(tmp, unit_frames.size()));
            }
        }
    }

    this->m_unit_width = 16;
    this->m_unit_height = 16;
    this->m_unit_moving_width = 24;
    this->m_unit_moving_height = 24;

    // Effects
    std::vector<std::string> effects(
        {"land_explosion", "air_explosion", "naval_explosion", "submarine_hide",
         "submarine_appear"});

    // Every effect sub data set
    for (size_t effect_idx = 0; effect_idx < effects.size(); effect_idx++)
    {
        HighFive::DataSet effect_ds = file.getDataSet("effects/" + effects[effect_idx]);

        std::vector<std::vector<std::vector<uint32_t>>> effect_frames;
        effect_ds.read(effect_frames);

        std::vector<uint32_t> effect_buffer(32 * 32 * effect_frames.size(), 0);

        // every animation frame
        for (size_t n = 0; n < effect_frames.size(); n++)
        {
            for (size_t y = 0; y < 32; y++)
            {
                for (size_t x = 0; x < 32; x++)
                {
                    size_t index = (y * effect_frames.size() * 32) + (n * 32 + x);

                    effect_buffer.at(index) = effect_frames.at(n).at(32 - y - 1).at(x);
                }
            }
        }

        SDL_Texture* tmp = SDL_CreateTexture(
            engine.renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
            effect_frames.size() * 32, 32);

        SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);

        if (tmp == nullptr)
        {
            throw std::runtime_error(
                "Fehler beim Erstellen der Textur für die Effects: " + std::string(SDL_GetError()));
        }

        if (SDL_UpdateTexture(
                tmp, NULL, effect_buffer.data(), effect_frames.size() * 32 * sizeof(int32_t)) != 0)
        {
            throw std::runtime_error(
                "Fehler beim updaten der Textur für die Tiles: " + std::string(SDL_GetError()));
        }

        m_effect_textures.push_back(std::pair<SDL_Texture*, int>(tmp, effect_frames.size()));
    }

    this->m_effect_width = 32;
    this->m_effect_height = 32;
}

// Tiles

int Spritesheet::get_tile_width()
{
    return m_tile_width;
}

int Spritesheet::get_tile_height()
{
    return m_tile_height;
}

std::vector<std::pair<SDL_Texture*, int>>& Spritesheet::get_tile_textures()
{
    return m_tile_textures;
}

// Buildings
int Spritesheet::get_building_width()
{
    return this->m_building_width;
}

int Spritesheet::get_building_height()
{
    return this->m_building_height;
}

std::vector<SDL_Texture*>& Spritesheet::get_building_textures()
{
    return m_building_textures;
}

// Units
int Spritesheet::get_unit_width()
{
    return this->m_unit_width;
}

int Spritesheet::get_unit_height()
{
    return this->m_unit_height;
}

int Spritesheet::get_unit_moving_width()
{
    return this->m_unit_moving_width;
}

int Spritesheet::get_unit_moving_height()
{
    return this->m_unit_moving_height;
}

std::vector<std::vector<std::vector<std::pair<SDL_Texture*, int>>>>&
Spritesheet::get_unit_textures()
{
    return this->m_unit_textures;
}

// Effects
int Spritesheet::get_effect_width()
{
    return this->m_effect_width;
}

int Spritesheet::get_effect_height()
{
    return this->m_effect_height;
}

std::vector<std::pair<SDL_Texture*, int>>& Spritesheet::get_effect_textures()
{
    return this->m_effect_textures;
}

Spritesheet::~Spritesheet()
{
    for (std::pair<SDL_Texture*, int> tile_texture : m_tile_textures)
    {
        SDL_DestroyTexture(tile_texture.first);
    }

    for (SDL_Texture* building_texture : m_building_textures)
    {
        SDL_DestroyTexture(building_texture);
    }

    for (std::vector<std::vector<std::pair<SDL_Texture*, int>>> faction : m_unit_textures)
    {
        for (std::vector<std::pair<SDL_Texture*, int>> unit : faction)
        {
            for (std::pair<SDL_Texture*, int> state : unit)
            {
                SDL_DestroyTexture(state.first);
            }
        }
    }
}

} // namespace advanced_wars