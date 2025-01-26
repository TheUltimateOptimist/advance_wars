#include "unit.hpp"

namespace advanced_wars {

Unit::Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state)
    : x(x), y(y), faction(faction), id(id), state(state) {

      };

void Unit::render(Engine &engine, int scale) {
  Spritesheet *spritesheet = engine.get_spritesheet();

  int step = engine.get_stage() % spritesheet->get_unit_textures()
                                      .at(static_cast<int>(faction))
                                      .at(static_cast<int>(id))
                                      .at(static_cast<int>(state))
                                      .second;

  if (state == UnitState::IDLE || state == UnitState::UNAVAILABLE) {

    SDL_Rect src;
    src.x = step * spritesheet->get_unit_width();
    src.y = 0;
    src.w = spritesheet->get_unit_width();
    src.h = spritesheet->get_unit_height();

    SDL_Rect dst;
    dst.x = x * spritesheet->get_unit_width() * scale;
    dst.y = y * spritesheet->get_unit_height() * scale;
    dst.w = spritesheet->get_unit_width() * scale;
    dst.h = spritesheet->get_unit_height() * scale;

    SDL_RenderCopyEx(engine.renderer(),
                     spritesheet->get_unit_textures()
                         .at(static_cast<int>(faction))
                         .at(static_cast<int>(id))
                         .at(static_cast<int>(state))
                         .first,
                     &src, &dst, 0, NULL, SDL_FLIP_NONE);
  } else {
    // The moving states have a resolution of 24x24 instead of 16x16 and need to
    // be handled separately
    SDL_Rect src;
    src.x = step * spritesheet->get_unit_moving_width();
    src.y = 0;
    src.w = spritesheet->get_unit_moving_width();
    src.h = spritesheet->get_unit_moving_height();

    SDL_Rect dst;
    dst.x = ((x * spritesheet->get_unit_width()) - 4) * scale;
    dst.y = ((y * spritesheet->get_unit_height()) - 4) * scale;
    dst.w = spritesheet->get_unit_moving_width() * scale;
    dst.h = spritesheet->get_unit_moving_height() * scale;

    SDL_RenderCopyEx(engine.renderer(),
                     spritesheet->get_unit_textures()
                         .at(static_cast<int>(faction))
                         .at(static_cast<int>(id))
                         .at(static_cast<int>(state))
                         .first,
                     &src, &dst, 0, NULL, SDL_FLIP_NONE);
  }
}

} // namespace advanced_wars