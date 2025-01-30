#include "unit.hpp"
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
    
    void Unit::attack(Unit* enemy) {

        int offDamage = 50;
        int defDamage = 50;

        enemy->health = enemy->health - offDamage;
        std::cout << "Enemy health:" << enemy->health << std::endl;
        if (enemy->health > 0) {
            this->health = this->health - defDamage;
            std::cout << "Health ally:" << this->health << std::endl;
        }
    }


    void Unit::update_position(int posX, int posY)
    {
        calc_state(posX, posY);

        this->x = posX;
        this->y = posY;
    }

    void Unit::calc_state(int posX, int posY) {

        int currentX = this->x;
        int currentY = this->y;

        int deltaX = currentX - posX;
        int deltaY = currentY - posY;

        if (deltaY == 0) {
            if (deltaX > 0) {
                this->state = advanced_wars::UnitState::MOVEMENTLEFT;
                return;
            } else {
                this->state = advanced_wars::UnitState::MOVEMENTRIGHT;
                return;
            }
        }

        double bresen = deltaX/deltaY;

        if(bresen == 0) {
            if(deltaY < 0) {
                this->state = advanced_wars::UnitState::MOVEMENTDOWN;
                return;
            } else {
                this->state = advanced_wars::UnitState::MOVEMENTUP;
                return;
            }
        }

        if(0 < bresen && bresen < 1) {
            this->state = advanced_wars::UnitState::MOVEMENTDOWN;
            return;
        } else if (-1 < bresen && bresen < 0) {
            this->state = advanced_wars::UnitState::MOVEMENTUP;
            return;
        }

    }

    void Unit::on_left_click(SDL_Event event, std::vector<Unit> &unitVector)
    {

        std::cout << "Left-button pressed on unit: " << this->health << std::endl;
        
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

} // namespace advanced_wars