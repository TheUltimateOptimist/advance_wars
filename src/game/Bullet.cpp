/**
 * Spritesheet.hpp
 *
 * @date 07.02.2025
 * @author David Hermann
 */

#include "Bullet.hpp"
#include <cmath>

namespace advanced_wars
{

void Bullet::render(SDL_Renderer* renderer, int scale, int bulletWidth, int bulletHeight)
{
    if (!m_texture)
        return;

    SDL_Rect src = {0, 0, bulletWidth, bulletHeight};

    // Berechne das Zielrechteck (Destination)
    SDL_Rect dst;
    dst.x = static_cast<int>(m_currentX * scale) - (bulletWidth * scale) / 2;
    dst.y = static_cast<int>(m_currentY * scale) - (bulletHeight * scale) / 2;
    dst.w = bulletWidth * scale;
    dst.h = bulletHeight * scale;

    // Berechne den Winkel (in Grad) basierend auf der Flugrichtung
    float dx = m_targetX - m_startX;
    float dy = m_targetY - m_startY;
    float angle = std::atan2(dy, dx) * 180.0f / M_PI;

    // Setze das Rotationszentrum (z. B. die Mitte des Destinationsrechtecks)
    SDL_Point center = {dst.w / 2, dst.h / 2};

    // Rendern mit Rotation; flip ist hier nicht nötig, da der Winkel das übernimmt
    SDL_RenderCopyEx(renderer, m_texture, &src, &dst, angle, &center, SDL_FLIP_NONE);
}

void Bullet::update(float deltaTime)
{
    if (m_finished)
        return;

    m_elapsedTime += deltaTime;
    float t = m_elapsedTime / m_duration;
    if (t >= 1.0f)
    {
        t = 1.0f;
        m_finished = true;
    }
    // Lineare Interpolation
    m_currentX = m_startX + t * (m_targetX - m_startX);
    m_currentY = m_startY + t * (m_targetY - m_startY);
}

} // namespace advanced_wars