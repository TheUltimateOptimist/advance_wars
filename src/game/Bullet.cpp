#include "Bullet.hpp"
#include "Box2dHelper.hpp"
#include "Engine.hpp"
#include "UnitContactListener.hpp"
#include <iostream>
#include <stdexcept>

namespace advanced_wars
{

Bullet::Bullet(b2World& world, float startX, float startY, float velocityX, float velocityY)
    : m_renderX(0), m_renderY(0)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(startX / PIXELS_PER_METER, startY / PIXELS_PER_METER);
    bodyDef.linearVelocity.Set(velocityX, velocityY);

    m_body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(4.0f / PIXELS_PER_METER, 4.0f / PIXELS_PER_METER);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.1f;
    fixtureDef.restitution = 0.1f;
    fixtureDef.isSensor = true;

    m_body->CreateFixture(&fixtureDef);

    BodyUserData* bud = new BodyUserData();
    bud->type = BodyUserData::Type::Bullet;
    bud->data = this;
    m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(bud);

    this->update();

    std::cout << "Bullet erstellt bei (" << startX << ", " << startY << ")" << std::endl;
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
