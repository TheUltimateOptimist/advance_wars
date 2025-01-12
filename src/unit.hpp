#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>
#include <utility>

// Enumerationen für Bewegungstyp und Waffenart
enum class MovementType {
    Foot,
    Tracks,
    Wheels,
    Air,
    Shipengine   
};

enum class WeaponType {
    Gun,
    Rocket
};

// Deklaration der Unit-Klasse
class Unit {
public:
    // Konstruktor
    Unit(std::string name, int health, int speed, int attack, std::pair<int,int> range, MovementType movement, WeaponType weapon);

    // Getter-Methoden
    std::string getName() const;
    int getHealth() const;
    int getSpeed() const;
    int getAttack() const;
    std::pair<int, int> getRange() const;
    MovementType getMovementType() const;
    WeaponType getWeaponType() const;
    int getX() const;
    int getY() const;

    // Setter-Methoden
    void setPosition(int newX, int newY);
    void setHealth(int newHealth);
    void setSpeed(int newSpeed);

private:
    // Private Membervariablen der Einheit
    std::string name;
    int health;
    int speed;
    int attack;
    std::pair<int, int> range; // Angriffsreichweite
    MovementType movement;
    WeaponType weapon;
    int x; // Position auf der x-Achse
    int y; // Position auf der y-Achse
};

#endif // UNIT_HPP