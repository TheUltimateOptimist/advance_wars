#include "spritesheet.hpp"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "engine.hpp"

#include <SDL_image.h>
#include <stdexcept>

namespace advanced_wars
{

  Spritesheet::Spritesheet(std::string path, Engine &engine)
  {
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr)
    {
      throw std::runtime_error("Fehler beim Laden des Bildes " + path + ": " +
                               std::string(IMG_GetError()));
    }

    texture = SDL_CreateTextureFromSurface(engine.renderer(), loadedSurface);

    if (texture == nullptr)
    {
      throw std::runtime_error("Fehler beim Erstellen der Textur: " +
                               std::string(SDL_GetError()));
    }

    SDL_FreeSurface(loadedSurface);

    // Temporary
    this->tile_width = 16;
    this->tile_height = 16;
    this->tiles.push_back(std::pair(0, 1));
    this->tiles.push_back(std::pair(1, 4));
  }

  int Spritesheet::get_tile_steps(int tile) { return tiles.at(tile).second; }

  int Spritesheet::get_tile_width() { return tile_width; }

  int Spritesheet::get_tile_height() { return tile_height; }

  int Spritesheet::render_tile(SDL_Renderer *renderer, int tile, int step,
                               SDL_Rect *rect)
  {
    if (step >= this->get_tile_steps(tile) || step < 0)
    {
      throw std::runtime_error("Tried to access step " + std::to_string(step) +
                               " for tile " + std::to_string(tile));
    }

    SDL_Rect src;
    src.x = tile * tile_width;
    src.y = 0;
    src.w = tile_width;
    src.h = tile_height;

    /*
       std::cout << rect->x << " " << rect->y << " " << rect->w << " " << rect->h
                << std::endl;
      */

    return SDL_RenderCopyEx(renderer, texture, &src, rect, 0, NULL,
                            SDL_FLIP_NONE);
  }

  Spritesheet::~Spritesheet() { SDL_DestroyTexture(texture); }

} // namespace advanced_wars