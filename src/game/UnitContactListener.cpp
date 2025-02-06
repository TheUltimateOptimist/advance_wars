#include "UnitContactListener.hpp"
#include "Level.hpp"
#include <iostream>

namespace advanced_wars
{

UnitContactListener::UnitContactListener(Level* level) : m_level(level) {}

void UnitContactListener::BeginContact(b2Contact* contact)
{
    std::cout << "BeginContact called" << std::endl;

    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Lese die benutzerdefinierten Daten der Bodies aus
    auto* userDataA = reinterpret_cast<BodyUserData*>(bodyA->GetUserData().pointer);
    auto* userDataB = reinterpret_cast<BodyUserData*>(bodyB->GetUserData().pointer);

    std::cout << "Body A userData pointer: " << userDataA << std::endl;
    std::cout << "Body B userData pointer: " << userDataB << std::endl;

    if (!userDataA || !userDataB)
    {
        std::cout << "Mindestens ein Body hat kein userData (wird ignoriert)." << std::endl;
        return;
    }

    std::cout << "Body A type: " << static_cast<int>(userDataA->type) << std::endl;
    std::cout << "Body B type: " << static_cast<int>(userDataB->type) << std::endl;
    std::cout << "Map Id A: " << userDataA->uniqueId << std::endl;
    std::cout << "Map Id B: " << userDataB->uniqueId << std::endl;

    // Prüfe, ob es sich um eine Bullet-Unit-Kollision handelt
    if (userDataA->type == BodyUserData::Type::Bullet &&
        userDataB->type == BodyUserData::Type::Unit)
    {
        std::cout
            << "Kollision erkannt: Bullet trifft auf Unit (Body A ist Bullet, Body B ist Unit): "
            << std::endl;
        m_level->checkBulletCollision(*static_cast<Unit*>(userDataB->data));
        // m_level->markBulletForRemoval();
        //  m_level->removeBullet();
    }
    else if (
        userDataB->type == BodyUserData::Type::Bullet &&
        userDataA->type == BodyUserData::Type::Unit)
    {
        std::cout
            << "Kollision erkannt: Bullet trifft auf Unit (Body B ist Bullet, Body A ist Unit)!"
            << std::endl;
        m_level->checkBulletCollision(*static_cast<Unit*>(userDataA->data));
        // m_level->markBulletForRemoval();
        //  m_level->removeBullet();
    }
    else
    {
        std::cout << "Kontakt, aber keine Bullet-Unit-Kollision." << std::endl;
    }
}

} // namespace advanced_wars
