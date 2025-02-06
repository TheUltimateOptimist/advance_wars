#ifndef UNIT_CONTACT_LISTENER_HPP
#define UNIT_CONTACT_LISTENER_HPP

#include "Bullet.hpp"
#include "Unit.hpp"
#include <box2d/box2d.h>

namespace advanced_wars
{

class Level; // Forward declaration

struct BodyUserData
{
        enum class Type
        {
            Unit = 0,
            Bullet = 1,
            Static = 2 // TODO Später evtl. für Buildings
        };
        Type  type;
        void* data; // (z. B. Unit* oder Bullet*)
        int   uniqueId;
};

class UnitContactListener : public b2ContactListener
{
    public:
        UnitContactListener(Level* level);

        void BeginContact(b2Contact* contact) override;

    private:
        Level* m_level;
};

} // namespace advanced_wars

#endif