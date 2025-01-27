#include "level.hpp"
#include "SDL_error.h"
#include "building.hpp"
#include "effect.hpp"
#include "engine.hpp"
#include "highfive/H5File.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "spritesheet.hpp"
#include "unit.hpp"
#include <SDL.h>
#include <iostream>
#include <string>

namespace advanced_wars {

Level::Level(std::string name, int width, int height, std::vector<Tile> tiles,
             std::vector<Building> buildings, std::vector<Unit> units,
             std::vector<Effect> effects)
    : name(name), width(width), height(height), tiles(tiles),
      buildings(buildings), units(units), effects(effects) {

  if ((size_t)(width * height) != tiles.size()) {
    throw std::runtime_error("level tile mismatch");
  }
};

  Level Level::loadLevel(std::string path)
{
  HighFive::File file(path, HighFive::File::ReadOnly);

  // read level metadata
  std::string level_metadata;
  file.getDataSet("level/metadata").read(level_metadata);

  // read tilesarray
  std::vector<uint8_t> level_tilesarray;
  file.getDataSet("level/tilesarray").read(level_tilesarray);

  // extract metadata from xml
  std::istringstream xmlStream(level_metadata);
  boost::property_tree::ptree pt;
  boost::property_tree::read_xml(xmlStream, pt);
  int width = pt.get<int>("level.width");
  int height = pt.get<int>("level.height");
  std::string name = pt.get<std::string>("level.name");
  std::vector<Tile> tiles;
  tiles.reserve(width*height);
  for (uint8_t value : level_tilesarray)
  {
    tiles.push_back(Tile())
  }


  throw std::runtime_error("some");
};

void Level::render(Engine &engine, std::vector<SDL_Event> &events) {
  const int RENDERING_SCALE = 3;

  // Iterate over all events
  while (!events.empty()) {
    events.erase(events.begin());
  }

  // Tiles
  for (Tile &tile : tiles) {
    tile.render(engine, RENDERING_SCALE);
  }

  // Buildings
  for (Building &building : buildings) {
    building.render(engine, RENDERING_SCALE);
  }

  // Units
  for (Unit &unit : units) {
    unit.render(engine, RENDERING_SCALE);
  }

  // Effects
  for (Effect &effect : effects) {
    effect.render(engine, RENDERING_SCALE);
  }

  // Set background color for renderer
  if (SDL_SetRenderDrawColor(engine.renderer(), 255, 0, 0, 0)) {
    std::cout << "Could not set render draw color: " << SDL_GetError()
              << std::endl;
  }
}

} // namespace advanced_wars
