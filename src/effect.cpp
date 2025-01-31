#include "effect.hpp"
#include "spritesheet.hpp"
#include <vector>

namespace advanced_wars
{

Effect::Effect(int x, int y, EffectId id, bool repeat)
    : m_x(x), m_y(y), m_id(id), m_repeat(repeat), m_start(0){

                                          };

void Effect::render(Engine* engine, int scale)
{
    Spritesheet* spritesheet = engine->get_spritesheet();
    if (m_start == 0)
    {
        m_start = engine->get_stage();
    }

    int step =
        engine->get_stage() % spritesheet->get_effect_textures().at(static_cast<int>(m_id)).second;

    SDL_Rect src;
    src.x = step * spritesheet->get_effect_width() + step * spritesheet->get_effect_height();
    src.y = 0;
    src.w = spritesheet->get_effect_width();
    src.h = spritesheet->get_effect_height();

    SDL_Rect dest;
    dest.x = (m_x * spritesheet->get_tile_width() * scale) - 8;
    dest.y = (m_y * spritesheet->get_tile_height() * scale) - 8;
    dest.w = spritesheet->get_effect_width() * scale;
    dest.h = spritesheet->get_effect_height() * scale;

    SDL_RenderCopyEx(
        engine->renderer(), spritesheet->get_effect_textures().at(static_cast<int>(m_id)).first, &src,
        &dest, 0, NULL, SDL_FLIP_NONE);
}

bool Effect::is_finished(Engine* engine)
{
    return !(
        engine->get_stage() - m_start <=
            engine->get_spritesheet()->get_effect_textures().at(static_cast<int>(m_id)).second ||
        m_repeat);
}

} // namespace advanced_wars