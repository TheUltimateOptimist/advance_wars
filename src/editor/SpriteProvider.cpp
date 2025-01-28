#include "SpriteProvider.hpp"

SpriteProvider::SpriteProvider(std::vector<QPixmap> sprites) : sprites(sprites){}

QPixmap SpriteProvider::get_sprite(uint8_t id) {
    QImage image(16, 16, QImage::Format::Format_ARGB32);
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            image.setPixel(x, y, 0xFF228B22);
        }
    }
    return QPixmap::fromImage(image);
}

SpriteProvider SpriteProvider::from_spritesheet(const std::string path)
{
    return SpriteProvider({});
}
