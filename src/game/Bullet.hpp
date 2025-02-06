#ifndef BULLET_HPP
#define BULLET_HPP

#include "Engine.hpp"
#include <SDL.h>
#include <box2d/box2d.h>

namespace advanced_wars
{

class Bullet
{
    public:
        /**
         * Erzeugt eine neue Bullet.
         *
         * @param world     Referenz auf die Box2D-Welt, in der der Body angelegt wird.
         * @param startX    Startposition X in Pixeln.
         * @param startY    Startposition Y in Pixeln.
         * @param velocityX Anfangsgeschwindigkeit X (in m/s, Box2D-Einheiten).
         * @param velocityY Anfangsgeschwindigkeit Y (in m/s, Box2D-Einheiten).
         * @param texture   SDL_Texture*, die über dein Spritesheet geliefert wird.
         * @param width     Breite der Bullet in Pixeln.
         * @param height    Höhe der Bullet in Pixeln.
         */
        Bullet(
            std::shared_ptr<b2World> world, float startX, float startY, float velocityX,
            float velocityY);

        ~Bullet();

        /// Update: Liest die Position des Box2D-Bodies und bereitet die Renderkoordinaten vor.
        void update();

        /// Rendert die Bullet mit SDL.
        void render(Engine& engine, int scale);

        /// Gibt den Box2D-Body zurück (nützlich z.B. für Kollisionsbehandlung)
        b2Body* getBody() const;

    private:
        b2Body*      m_body;    // Box2D-Körper der Bullet
        SDL_Texture* m_texture; // Textur, die über das Spritesheet bezogen wird
        int          m_width;   // Breite in Pixeln
        int          m_height;  // Höhe in Pixeln

        // Gerenderte Position in Pixeln (berechnet aus der Box2D-Position)
        float m_renderX;
        float m_renderY;
};
} // namespace advanced_wars

#endif // BULLET_HPP
