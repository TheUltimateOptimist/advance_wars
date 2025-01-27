#include "TileProvider.hpp"

TileProvider::TileProvider(std::vector<std::vector<uint32_t>> sprites) : sprites(sprites){}

QPixmap TileProvider::get_tile(uint8_t id) {
    QImage image(16, 16, QImage::Format::Format_ARGB32);
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            image.setPixel(x, y, 0xFF228B22);
        }
    }
    return QPixmap::fromImage(image);
}

TileProvider TileProvider::from_spritesheet(const std::string path)
{
    return TileProvider({});
}
