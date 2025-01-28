#include <QPixmap>
#include <vector>
#include "highfive/H5File.hpp"

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

const std::vector<std::string> faction_names(
      {"red", "blue", "yellow", "green", "purple", "neutral"});

class SpriteProvider {
public: 
    SpriteProvider() = delete;
    static QPixmap get_sprite(uint8_t id);
    static void initialize(const std::string& path);

private:
    static QPixmap load_pixmap(std::vector<std::vector<std::vector<uint32_t>>> pixels, int index);
    static std::vector<QPixmap> sprites;
};