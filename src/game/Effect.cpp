#include "Effect.hpp"
#include "Spritesheet.hpp"
#include <SDL.h>
#include <iostream>
#include <vector>

namespace advanced_wars
{

Effect::Effect(int x, int y, EffectId id, bool repeat)
    : m_x(x), m_y(y), m_id(id), m_repeat(repeat), m_start(SDL_GetTicks())
{
}

void Effect::render(Engine& engine, int scale)
{
    Spritesheet* spritesheet = engine.getSpritesheet();
    // m_start wird jetzt bereits im Konstruktor gesetzt – kein if (m_start == 0) mehr nötig.

    int frameCount = spritesheet->getEffectTextures().at(static_cast<int>(m_id)).second;
    int step = engine.getStage() % frameCount;

    SDL_Rect src;
    src.x = step * spritesheet->getEffectWidth();
    src.y = 0;
    src.w = spritesheet->getEffectWidth();
    src.h = spritesheet->getEffectHeight();

    int      tileWidth = spritesheet->getTileWidth();
    int      tileHeight = spritesheet->getTileHeight();
    SDL_Rect dest;
    dest.x = (m_x * tileWidth * scale) - (spritesheet->getEffectWidth() * scale) / 2;
    dest.y = (m_y * tileHeight * scale) - (spritesheet->getEffectHeight() * scale) / 2;
    dest.w = spritesheet->getEffectWidth() * scale;
    dest.h = spritesheet->getEffectHeight() * scale;

    std::cout << "Rendering effect (id=" << static_cast<int>(m_id) << "): "
              << "dest.x=" << dest.x << " dest.y=" << dest.y << " dest.w=" << dest.w
              << " dest.h=" << dest.h << std::endl;

    SDL_RenderCopyEx(
        engine.renderer(), spritesheet->getEffectTextures().at(static_cast<int>(m_id)).first, &src,
        &dest, 0, NULL, SDL_FLIP_NONE);
}

bool Effect::is_finished(Engine& engine)
{
    // Falls der Effekt wiederholt werden soll, läuft er unendlich
    if (m_repeat)
    {
        return false;
    }

    // Berechne die Gesamtdauer in Millisekunden:
    // Wir gehen davon aus, dass jeder Frame 300ms angezeigt wird.
    int frameCount = engine.getSpritesheet()->getEffectTextures().at(static_cast<int>(m_id)).second;
    int duration = frameCount * 300;

    // Debug-Ausgabe:
    int elapsed = SDL_GetTicks() - m_start;
    std::cout << "Effect (id=" << static_cast<int>(m_id) << ") elapsed=" << elapsed
              << " ms / duration=" << duration << " ms" << std::endl;

    return elapsed > duration;
}

} // namespace advanced_wars
