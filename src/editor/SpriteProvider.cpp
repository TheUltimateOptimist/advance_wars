#include "SpriteProvider.hpp"
#include <iostream>

SpriteProvider::SpriteProvider(std::vector<QPixmap> sprites) : sprites(sprites){}

QPixmap SpriteProvider::get_sprite(uint8_t id) {
    if (id >= 50) {
        return sprites[id - 20];
    }
    return sprites[id];
}

SpriteProvider SpriteProvider::from_spritesheet(const std::string path)
{
    HighFive::File file(path, HighFive::File::ReadOnly);
    std::vector<QPixmap> sprites;
    sprites.reserve(60);

    //load terrains
    for (size_t i = 0; i < tile_names.size(); i++) {
        std::vector<std::vector<std::vector<uint32_t>>> pixels;
        file.getDataSet("tiles/" + tile_names[i]).read(pixels);
        sprites.push_back(SpriteProvider::load_pixmap(pixels, 0));
    }

    // load buildings
    for (const std::string& faction_name : faction_names) {
        std::vector<std::vector<std::vector<uint32_t>>> pixels;
        file.getDataSet("buildings/" + faction_name).read(pixels);
        for (int i = 0; i < 5; i++) {
            sprites.push_back(SpriteProvider::load_pixmap(pixels, i));
        } 
    }
    return SpriteProvider(sprites);
}

QPixmap SpriteProvider::load_pixmap(std::vector<std::vector<std::vector<uint32_t>>> pixels, int index) {
    int width = pixels[index][0].size();
    int height = pixels[index].size();
    QImage image(width, height, QImage::Format::Format_RGBA8888);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint32_t color = pixels[index][height - 1 - y][x];
            uint32_t r = (color >> 24) & 0xFF; // Most significant byte
            uint32_t g = (color >> 16) & 0xFF; // Second byte
            uint32_t b = (color >> 8)  & 0xFF; // Third byte
            uint32_t a = color & 0xFF;         // Least significant byte 
            image.setPixel(x, y, qRgba(r, g, b, a));
        }
    } 
    return QPixmap::fromImage(image);
}