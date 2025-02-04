#include "Bullet.hpp"
#include "Engine.hpp"
#include <stdexcept>

namespace advanced_wars
{

Bullet::Bullet(b2World& world, float startX, float startY, float velocityX, float velocityY)
    : m_renderX(0), m_renderY(0)
{
}

Bullet::~Bullet()
{
    // Das Zerstören des Box2D-Körpers sollte über die Welt erfolgen.
    // Daher wird hier kein direktes Delete benötigt.
    // (Die Bullet-Entfernung aus der Welt sollte an anderer Stelle erfolgen.)
}

void Bullet::update()
{
    // Hole die aktuelle Position des Körpers in Box2D-Einheiten (Meter)
    b2Vec2 pos = m_body->GetPosition();
    // Umrechnung in Pixel; dabei wird der Mittelpunkt des Körpers ermittelt.
    m_renderX = pos.x * PIXELS_PER_METER - m_width / 2.0f;
    m_renderY = pos.y * PIXELS_PER_METER - m_height / 2.0f;
}

void Bullet::render(Engine& engine, int scale)
{
    // Falls die Textur noch nicht gesetzt wurde, laden wir sie jetzt
    if (!m_texture)
    {
        Spritesheet* spritesheet = engine.getSpritesheet();
        m_texture = spritesheet->getBulletTexture(); // Textur aus HDF5-File laden
        m_width = spritesheet->getBulletWidth();
        m_height = spritesheet->getBulletHeight();
    }

    // Ziel-Rect setzen (angepasst für Skalierung)
    SDL_Rect destRect = {
        static_cast<int>(m_renderX * scale), static_cast<int>(m_renderY * scale), m_width * scale,
        m_height * scale};

    // Textur rendern
    SDL_RenderCopy(engine.renderer(), m_texture, nullptr, &destRect);
}

b2Body* Bullet::getBody() const
{
    return m_body;
}

} // namespace advanced_wars
