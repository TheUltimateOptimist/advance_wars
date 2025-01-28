#include "effect.hpp"
#include "spritesheet.hpp"
#include <vector>

namespace advanced_wars
{

Effect::Effect(int x, int y, EffectId id, bool repeat)
    : x(x), y(y), id(id), repeat(repeat), start(0){

                                          };

void Effect::render(Engine& engine, int scale)
{
    Spritesheet* spritesheet = engine.get_spritesheet();
    if (start == 0)
    {
        start = engine.get_stage();
    }

    int step =
        engine.get_stage() % spritesheet->get_effect_textures().at(static_cast<int>(id)).second;

    if (engine.get_stage() - start <=
            spritesheet->get_effect_textures().at(static_cast<int>(id)).second ||
        repeat)
    {
        SDL_Rect src;
        src.x = step * spritesheet->get_effect_width() + step * spritesheet->get_effect_height();
        src.y = 0;
        src.w = spritesheet->get_effect_width();
        src.h = spritesheet->get_effect_height();

        SDL_Rect dest;
        dest.x = (x * spritesheet->get_tile_width() * scale) - 8;
        dest.y = (y * spritesheet->get_tile_height() * scale) - 8;
        dest.w = spritesheet->get_effect_width() * scale;
        dest.h = spritesheet->get_effect_height() * scale;

        SDL_RenderCopyEx(
            engine.renderer(), spritesheet->get_effect_textures().at(static_cast<int>(id)).first,
            &src, &dest, 0, NULL, SDL_FLIP_NONE);
    }
}

} // namespace advanced_wars