/**
* SpriteProvider.cpp
*
* @date 27.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "SpriteProvider.hpp"

#include <iostream>
#include <stdexcept>

namespace editor
{

std::vector<QPixmap> SpriteProvider::sprites = std::vector<QPixmap>();

QPixmap SpriteProvider::get_sprite(uint8_t id)
{
    if (sprites.empty())
    {
        throw std::runtime_error("Sprites accessed before initilization!");
    }
    if (id >= 50)
    {
        return SpriteProvider::sprites[id - 20]; // theres a 20 gap between floor tiles 0-29
                                                 // and building tiles 50-79
    }
    return SpriteProvider::sprites[id];
}

void SpriteProvider::initialize(const std::string& path)
{
    // check if sprites have already been initialized
    if (sprites.size() > 0)
    {
        return;
    }

/**
 * Provides all terrain tile names.
 * They are necessary to create the correct path to 
 * the hdf5 dataset that contains the pixels for a given terrain tile.
 */
const std::vector<std::string> tile_names({"plain",
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

    /**
 * Provides the names for all factions in the game.
 * The names can be used to create the path to the hdf5 dataset
 * containing the buildings for a given faction.
 */
const std::vector<std::string> faction_names(
      {"red", "blue", "green", "yellow", "purple", "neutral"});

    HighFive::File file(path, HighFive::File::ReadOnly);
    sprites.reserve(60); // we now that we will load 60 sprites

    // load terrains
    for (size_t i = 0; i < tile_names.size(); i++)
    { // size_t?
        std::vector<std::vector<std::vector<uint32_t>>> pixels;
        file.getDataSet("tiles/" + tile_names[i]).read(pixels);
        sprites.push_back(SpriteProvider::load_pixmap(pixels, 0));
    }

    // load buildings
    for (const std::string& faction_name : faction_names)
    {
        std::vector<std::vector<std::vector<uint32_t>>> pixels;
        file.getDataSet("buildings/" + faction_name).read(pixels);
        for (int i = 0; i < 5; i++)
        {
            sprites.push_back(SpriteProvider::load_pixmap(pixels, i));
        }
    }
}

QPixmap SpriteProvider::load_pixmap(std::vector<std::vector<std::vector<uint32_t>>> pixels, int index)
{
    int    width = pixels[index][0].size();
    int    height = pixels[index].size();
    QImage image(width, height, QImage::Format::Format_RGBA8888);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uint32_t color = pixels[index][height - 1 - y][x];
            uint32_t r = (color >> 24) & 0xFF;
            uint32_t g = (color >> 16) & 0xFF;
            uint32_t b = (color >> 8) & 0xFF;
            uint32_t a = color & 0xFF;
            image.setPixel(x, y, qRgba(r, g, b, a));
        }
    }
    return QPixmap::fromImage(image);
}

} // namespace editor