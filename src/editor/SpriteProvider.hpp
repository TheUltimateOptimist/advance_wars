/**
* SpriteProvider.hpp
*
* @date 27.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QPixmap>
#include <vector>

#include "highfive/H5File.hpp"

namespace editor
{

/**
 * Provides all tile names.
 * They are necessary to create the correct path to 
 * the hdf5 dataset that contains the pixels for a given tile.
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
      {"red", "blue", "yellow", "green", "purple", "neutral"});

/**
 * SpriteProvider is only used in a static context.
 * SpriteProvider allows us to get the QPixmap for a given tile type
 * from anywhere in the program using SpriteProvider::get_sprite(id);
 * When SpriteProvider is initialized, it loads all the Sprites from the hdf5 Spritesheet
 * and stores them in the static sprites variable.
 * From then on, fast access to any sprite as a QPixmap is possible.
 */
class SpriteProvider {
public:
    /**
     * SpriteProvider is only used in a static context
     */
    SpriteProvider() = delete;
    /**
     * Allows you to retrieve the QPixmap of any tile given its id.
     * @param id The id of the tile type whose sprite you want to get.
     * @return The QPixmap for the provided tile id.
     */
    static QPixmap get_sprite(uint8_t id);
    /**
     * Always call initialize at the start of the program, to load
     * all of the sprites from the Spritesheet under the provided path.
     * Stores all sprites of the provided Spritesheet for later quick retrieval.
     * @param path The path to the hdf5 spritesheet
     */
    static void initialize(const std::string& path);

private:
    /**
     * Creates a QPixmap from the given pixels.
     * The provided 3-dim pixels vector contains multiple sprites, which is where the third dimension
     * comes from.
     * With the index you can control which sprite from the pixels Vector to create.
     * @param pixels The 3-dim vector of pixel values.
     * @param index The index of the pixeldata in the 3-dim Vector to use.
     * @return The QPixmap object that was created from the provided pixels.
     */
    static QPixmap load_pixmap(std::vector<std::vector<std::vector<uint32_t>>> pixels, int index);
    /**
     * Holds all of the game's sprites for quick access.
     */
    static std::vector<QPixmap> sprites;
};

} // namespace editor