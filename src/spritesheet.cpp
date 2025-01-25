#include "spritesheet.hpp"
#include "SDL_pixels.h"
#include "SDL_surface.h"
#include "building.hpp"
#include "common.h"
#include "engine.hpp"
#include "highfive/H5File.hpp"
#include "highfive/highfive.hpp"
#include <SDL_image.h>
#include <SDL_render.h>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace advanced_wars {

Spritesheet::Spritesheet(std::string path, Engine &engine) {

  HighFive::File file(path, HighFive::File::ReadOnly);

  // Tiles
  HighFive::DataSet tile_frames_ds = file.getDataSet("tiles/frames");

  HighFive::DataSet tile_num_frames_ds = file.getDataSet("tiles/num_frames");

  std::vector<std::vector<std::vector<uint32_t>>> tile_frames;

  tile_frames_ds.read(tile_frames);

  std::vector<uint32_t> tile_num_frames;

  tile_num_frames_ds.read(tile_num_frames);

  std::vector<uint32_t> tile_buffer(16 * 16 * tile_frames.size(), 0);

  for (size_t n = 0; n < tile_frames.size(); n++) {
    for (size_t y = 0; y < 16; y++) {
      for (size_t x = 0; x < 16; x++) {
        size_t index = (y * tile_frames.size() * 16) + (n * 16 + x);

        tile_buffer.at(index) = tile_frames.at(n).at(y).at(x);
      }
    }
  }

  int count = 0;
  for (size_t n = 0; n < tile_num_frames.size(); n++) {
    this->tiles.push_back(std::pair(count, tile_num_frames.at(n)));

    count += tile_num_frames.at(n);
  }

  tile_texture = SDL_CreateTexture(engine.renderer(), SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_STATIC, count * 16, 16);

  if (tile_texture == nullptr) {
    throw std::runtime_error(
        "Fehler beim Erstellen der Textur für die Tiles: " +
        std::string(SDL_GetError()));
  }

  if (SDL_UpdateTexture(tile_texture, NULL, tile_buffer.data(),
                        count * 16 * sizeof(int32_t)) != 0) {
    throw std::runtime_error("Fehler beim updaten der Textur für die Tiles: " +
                             std::string(SDL_GetError()));
  }

  this->tile_width = 16;
  this->tile_height = 16;

  // Buildings
  std::vector<std::string> factions(
      {"red", "blue", "yellow", "green", "purple", "neutral"});

  for (std::string faction : factions) {
    HighFive::DataSet buildings_ds = file.getDataSet("buildings/" + faction);

    std::vector<std::vector<std::vector<uint32_t>>> buildings_frames;

    buildings_ds.read(buildings_frames);

    std::vector<uint32_t> building_buffer(32 * 16 * buildings_frames.size(), 0);

    for (size_t n = 0; n < buildings_frames.size(); n++) {
      for (size_t y = 0; y < 32; y++) {
        for (size_t x = 0; x < 16; x++) {
          size_t index = (y * buildings_frames.size() * 16) + (n * 16 + x);

          building_buffer.at(index) =
              buildings_frames.at(n).at(32 - y - 1).at(x);
        }
      }
    }

    SDL_Texture *tmp = SDL_CreateTexture(
        engine.renderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
        buildings_frames.size() * 16, 32);

    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);

    if (tmp == nullptr) {
      throw std::runtime_error(
          "Fehler beim Erstellen der Textur für die Tiles: " +
          std::string(SDL_GetError()));
    }

    if (SDL_UpdateTexture(tmp, NULL, building_buffer.data(),
                          buildings_frames.size() * 16 * sizeof(int32_t)) !=
        0) {
      throw std::runtime_error(
          "Fehler beim updaten der Textur für die Tiles: " +
          std::string(SDL_GetError()));
    }

    this->building_textures.push_back(tmp);
  }

  this->building_width = 16;
  this->building_height = 32;
}

// Tiles
int Spritesheet::get_tile_steps(int tile) { return tiles.at(tile).second; }

int Spritesheet::get_tile_width() { return tile_width; }

int Spritesheet::get_tile_height() { return tile_height; }

int Spritesheet::render_tile(SDL_Renderer *renderer, int tile, int step,
                             SDL_Rect *rect) {
  int max_steps = this->get_tile_steps(tile);

  if (step >= max_steps || step < 0) {
    throw std::runtime_error("Tried to access step " + std::to_string(step) +
                             " for tile " + std::to_string(tile));
  }

  int offset = tiles.at(tile).first;

  SDL_Rect src;
  src.x = offset * tile_width + step * tile_width;
  src.y = 0;
  src.w = tile_width;
  src.h = tile_height;

  return SDL_RenderCopyEx(renderer, tile_texture, &src, rect, 0, NULL,
                          SDL_FLIP_NONE);
}

// Buildings
int Spritesheet::get_building_width() { return this->building_width; }

int Spritesheet::get_building_height() { return this->building_height; }

int Spritesheet::render_building(SDL_Renderer *renderer, BuildingId id,
                                 Faction faction, SDL_Rect *rect) {
  SDL_Rect src;
  src.x = static_cast<int>(id) * this->get_building_width();
  src.y = 0;
  src.w = this->get_building_width();
  src.h = this->get_building_height();

  return SDL_RenderCopyEx(renderer,
                          building_textures[static_cast<int>(faction)], &src,
                          rect, 0, NULL, SDL_FLIP_NONE);
}

Spritesheet::~Spritesheet() { SDL_DestroyTexture(tile_texture); }

} // namespace advanced_wars