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
        throw std::runtime_error("Sprites accessed before initilization");
    }
    if (id >= 50)
    {
        return SpriteProvider::sprites[id - 20];
    }
    return SpriteProvider::sprites[id];
}

void SpriteProvider::initialize(const std::string& path)
{
    // Check ob bereits initialisiert
    if (sprites.size() > 0)
    {
        return;
    }

    HighFive::File file(path, HighFive::File::ReadOnly);
    sprites.reserve(60);

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

QPixmap
SpriteProvider::load_pixmap(std::vector<std::vector<std::vector<uint32_t>>> pixels, int index)
{
    int    width = pixels[index][0].size();
    int    height = pixels[index].size();
    QImage image(width, height, QImage::Format::Format_RGBA8888);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uint32_t color = pixels[index][height - 1 - y][x];
            uint32_t r = (color >> 24) & 0xFF; // Most significant byte     //uint8_t sollte reichen
            uint32_t g = (color >> 16) & 0xFF; // Second byte               //uint8_t sollte reichen
            uint32_t b = (color >> 8) & 0xFF;  // Third byte                //uint8_t sollte reichen
            uint32_t a = color & 0xFF;         // Least significant byte    //uint8_t sollte reichen
            image.setPixel(x, y, qRgba(r, g, b, a));
        }
    }
    return QPixmap::fromImage(image);
}

} // namespace editor