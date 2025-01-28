#include "unit.hpp"
#include <tinyxml2.h>
#include <iostream>

namespace advanced_wars
{

    Unit::Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state)
        : x(x), y(y), faction(faction), id(id), state(state), max_health(100)
    {
        health = max_health;
    };

    void Unit::render(Engine &engine, int scale)
    {
        Spritesheet *spritesheet = engine.get_spritesheet();

        int step = engine.get_stage() % spritesheet->get_unit_textures()
                                            .at(static_cast<int>(faction))
                                            .at(static_cast<int>(id))
                                            .at(static_cast<int>(state))
                                            .second;

        if (state == UnitState::IDLE || state == UnitState::UNAVAILABLE)
        {

            SDL_Rect src;
            src.x = step * spritesheet->get_unit_width();
            src.y = 0;
            src.w = spritesheet->get_unit_width();
            src.h = spritesheet->get_unit_height();

            SDL_Rect dst;
            dst.x = x * spritesheet->get_unit_width() * scale;
            dst.y = y * spritesheet->get_unit_height() * scale;
            dst.w = spritesheet->get_unit_width() * scale;
            dst.h = spritesheet->get_unit_height() * scale;

            SDL_RenderCopyEx(engine.renderer(),
                             spritesheet->get_unit_textures()
                                 .at(static_cast<int>(faction))
                                 .at(static_cast<int>(id))
                                 .at(static_cast<int>(state))
                                 .first,
                             &src, &dst, 0, NULL, SDL_FLIP_NONE);
        }
        else
        {
            // The moving states have a resolution of 24x24 instead of 16x16 and need to
            // be handled separately
            SDL_Rect src;
            src.x = step * spritesheet->get_unit_moving_width();
            src.y = 0;
            src.w = spritesheet->get_unit_moving_width();
            src.h = spritesheet->get_unit_moving_height();

            SDL_Rect dst;
            dst.x = ((x * spritesheet->get_unit_width()) - 4) * scale;
            dst.y = ((y * spritesheet->get_unit_height()) - 4) * scale;
            dst.w = spritesheet->get_unit_moving_width() * scale;
            dst.h = spritesheet->get_unit_moving_height() * scale;

            SDL_RenderCopyEx(engine.renderer(),
                             spritesheet->get_unit_textures()
                                 .at(static_cast<int>(faction))
                                 .at(static_cast<int>(id))
                                 .at(static_cast<int>(state))
                                 .first,
                             &src, &dst, 0, NULL, SDL_FLIP_NONE);
        }
    }

    MatchupTabel damageMatrix;
    std::vector<Unit*> units;
    
    void Unit::attack(Unit& enemy) {

        int offDamage = 50;
        int defDamage = 1000;

        enemy.health = enemy.health - offDamage;
        std::cout << "Enemy health:" << enemy.health << std::endl;
        if (enemy.health > 0) {
            this->health = this->health - defDamage;
            std::cout << "Health ally:" << this->health << std::endl;
        }
    }

    void Unit::update_position(int posX, int posY)
    {
        this->x = posX;
        this->y = posY;
    }

/*
Features:
//select unit 
    - show context menu
    - show move range
    - MAYBE show valid targets

//deselect unit

//attack unit
    - show context menu

*/
    void Unit::onClick(SDL_Event event, std::vector<Unit> &unitVector)
    {

        Unit *defender = nullptr;
        Unit *attacker = nullptr;

        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:

            // we have to re-initialize the unit.state (probably to idle)
            this->is_selected = true;
            std::cout << "I am selected!!" << std::endl;

            /*
            for (Unit *unit : units)
            {
                if (!inRange(unit))
                {
                    unit->state = advanced_wars::UnitState::UNAVAILABLE;
                };
            }
            */

           //make move range calc
            break;
        case SDL_BUTTON_RIGHT:

            this->is_targeted = true;
            std::cout << "I am targeted!!" << std::endl;

            for (Unit unit : unitVector)
            {
                if (unit.state == advanced_wars::UnitState::UNAVAILABLE)
                {
                    continue;
                }

                if (unit.is_selected)
                {
                    attacker = &unit;
                }

                if (unit.is_targeted)
                {
                    defender = &unit;
                }
            }

            if (attacker != nullptr && defender != nullptr)
            {
                //attack(attacker, defender);
                std::cout << "We are fighting!!" << std::endl;
                break;
            }
            else
            {
                std::cerr << "Angriff konnte nicht gestartet werden!" << std::endl;
                break;
            }
        }
    }

    bool Unit::inRange(Unit *enemy)
    {
        if (this->x == enemy->x)
        {
            return abs(this->y - enemy->y) <= this->range;
        }
        else if (this->y == enemy->y)
        {
            return abs(this->x - enemy->x) <= this->range;
        }
        return false;
    }
    /*
    void Unit::loadXML(const char *filename)
    {

        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS)
        {
            std::cerr << "Fehler beim Laden der XML-Datei!" << std::endl;
            return;
        }

        MatchupTabel damageMatrix;
        tinyxml2::XMLElement *unitElement = doc.FirstChildElement("Units")->FirstChildElement("Unit");

        // get all Units
        while (unitElement)
        {

            const u_int8_t UnitId = unitElement->Attribute("id");

            std::unordered_map<u_int8_t, int> attackValues;
            tinyxml2::XMLElement *attackElement = unitElement->FirstChildElement("Attack");

            // get all attack-values
            while (attackElement)
            {

                tinyxml2::XMLElement *attackTypeElement = attackElement->FirstChildElement();

                while (attackTypeElement)
                {
                    UnitId Unit_Id = static_cast<UnitId> attackTypeElement->Name(); // wenn das geht kauf ich maggus sein kochbuch

                    int attackValue = attackTypeElement->IntText();

                    attackValues[Unit_Id] = attackValue;

                    attackTypeElement = attackTypeElement->NextSiblingElement();
                }
            }

            damageMatrix[unitElement][attackValues];
            unitElement = unitElement->NextSiblingElement("Unit");
        }
    }
    */
} // namespace advanced_wars