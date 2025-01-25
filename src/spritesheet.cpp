#include "spritesheet.hpp"
#include "SDL_pixels.h"
#include "SDL_surface.h"
#include "engine.hpp"
#include "highfive/H5File.hpp"
#include "highfive/highfive.hpp"
#include <SDL_image.h>
#include <SDL_render.h>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace advanced_wars {

Spritesheet::Spritesheet(std::string path, Engine &engine) {

  HighFive::File file(path, HighFive::File::ReadOnly);

  HighFive::DataSet tile_frames_ds = file.getDataSet("tiles/frames");

  HighFive::DataSet tile_num_frames_ds = file.getDataSet("tiles/num_frames");

  std::vector<std::vector<std::vector<uint32_t>>> tile_frames;

  tile_frames_ds.read(tile_frames);

  std::vector<uint32_t> tile_num_frames;

  tile_num_frames_ds.read(tile_num_frames);

  std::vector<uint32_t> buffer(16 * 16 * tile_frames.size(), 0);

  for (size_t n = 0; n < tile_frames.size(); n++) {
    for (size_t y = 0; y < 16; y++) {
      for (size_t x = 0; x < 16; x++) {
        size_t index = (y * tile_frames.size() * 16) + (n * 16 + x);

        buffer.at(index) = tile_frames.at(n).at(y).at(x);
      }
    }
  }

  int count = 0;
  for (size_t n = 0; n < tile_num_frames.size(); n++) {
    this->tiles.push_back(std::pair(count, tile_num_frames.at(n)));

    count += tile_num_frames.at(n);
  }

  texture = SDL_CreateTexture(engine.renderer(), SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_STATIC, count * 16, 16);

  if (texture == nullptr) {
    throw std::runtime_error("Fehler beim Erstellen der Textur: " +
                             std::string(SDL_GetError()));
  }

  if (SDL_UpdateTexture(texture, NULL, buffer.data(),
                        count * 16 * sizeof(int32_t)) != 0) {
    throw std::runtime_error("Fehler beim updaten der Textur: " +
                             std::string(SDL_GetError()));
  }

  this->tile_width = 16;
  this->tile_height = 16;
}

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

  return SDL_RenderCopyEx(renderer, texture, &src, rect, 0, NULL,
                          SDL_FLIP_NONE);
}

Spritesheet::~Spritesheet() { SDL_DestroyTexture(texture); }

} // namespace advanced_wars