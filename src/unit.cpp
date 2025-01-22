#include "unit.hpp"

Unit::Unit(std::string name, int health, int speed, int attack, std::pair<int, int> range, MovementType movement, WeaponType weapon)
    : name(std::move(name)), health(health), speed(speed), attack(attack), range(range), movement(movement), weapon(weapon) {}


//TODO: IF necessary, implement scaling attack by remaining health
void Unit::init_combat(Unit defender)
{
    //Attacker gets first move, inflicts its damage upon the the defender
    defender.setHealth(defender.getHealth() - this.getAttack());

    //if defender survives, it gets to inflict its damage upon the attacker
    if(defender.getHealth() > 0) {
        this.setHealth(this.getHealth() - defender.getAttack());
    } else { //if ded, defender gets deleted
        defender.~Unit();
    }
}

void Unit::update_position(int posX, int posY)
{
    this.setX(posX);
    this.setY(posY);
}
