#include <QPixmap>
#include <vector>

class TileProvider {
public: 
    TileProvider(std::vector<std::vector<uint32_t>> sprites);
    QPixmap get_tile(uint8_t id);
    TileProvider static from_spritesheet(const std::string path);

private:
    std::vector<std::vector<uint32_t>> sprites;
};