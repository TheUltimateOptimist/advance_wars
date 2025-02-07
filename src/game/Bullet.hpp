/**
 * Spritesheet.hpp
 *
 * @date 07.02.2025
 * @author David Hermann
 */

#pragma once
#include <SDL.h>

namespace advanced_wars
{

class Bullet
{
    public:
        // Konstruktor: Start- und Zielposition (in Tilekoordinaten, hier als float, damit wir auch
        // Zentren mit 0.5 haben) sowie die Animationsdauer (in Sekunden) und den Pointer auf die
        // Bullet-Textur
        Bullet(
            float startX, float startY, float targetX, float targetY, float duration,
            SDL_Texture* texture)
            : m_startX(startX), m_startY(startY), m_targetX(targetX), m_targetY(targetY),
              m_currentX(startX), m_currentY(startY), m_duration(duration), m_elapsedTime(0.0f),
              m_finished(false), m_texture(texture)
        {
        }

        // Update-Methode: Interpoliert die Position anhand der verstrichenen Zeit
        void update(float deltaTime);

        // Rendert die Bullet. Hier rechnen wir (optional) mit einem Scale-Faktor und einer festen
        // Breite/Höhe (in Pixel)
        void render(SDL_Renderer* renderer, int scale, int bulletWidth, int bulletHeight);

        // Liefert zurück, ob die Bullet ihr Ziel erreicht hat
        bool isFinished() const { return m_finished; }

    private:
        float m_startX; // Startposition (in Tilekoordinaten, z. B. x + 0.5 für den Mittelpunkt)
        float m_startY; // Startposition (in Tilekoordinaten, z. B. y + 0.5 für den Mittelpunkt)
        float m_targetX;
        float m_targetY; // Zielposition (ebenfalls in Tilekoordinaten)
        float m_currentX;
        float m_currentY;    // Aktuelle interpolierte Position
        float m_duration;    // Gesamtdauer der Animation (in Sekunden)
        float m_elapsedTime; // Verstrichene Zeit seit Start der Animation
        bool  m_finished;    // Wird true, wenn die Bullet ihr Ziel erreicht hat
        SDL_Texture*
            m_texture; // Die Textur der Bullet, geladen aus der hdf5-Datei (z. B. über Engine)
};

} // namespace advanced_wars
