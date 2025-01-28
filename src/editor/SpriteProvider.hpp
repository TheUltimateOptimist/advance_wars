#include <QPixmap>
#include <vector>

class SpriteProvider {
public: 
    SpriteProvider(std::vector<QPixmap> sprites);
    QPixmap get_sprite(uint8_t id);
    SpriteProvider static from_spritesheet(const std::string path);

private:
    std::vector<QPixmap> sprites;
};