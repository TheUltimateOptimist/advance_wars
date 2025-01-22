#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>
#include <utility>
#include "tile.hpp"

// Enumerationen für Bewegungstyp und Waffenart
enum class MovementType
{
    INFANTRY,
    TRACKS,
    WHEELS,
    AIR,
    SHIPENIGNE
};

enum class WeaponType
{
    GUN,
    ROCKET
};

// Deklaration der Unit-Klasse
class Unit
{
public:
    // Konstruktor
    Unit(std::string name, int health, int speed, int attack, std::pair<int, int> range, MovementType movement, WeaponType weapon);

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

    // Definition der statischen Bewegungskosten
    const std::map<std::pair<MovementType, TerrainType>, int> MOVEMENT_COSTS = {
        {{MovementType::INFANTRY, TerrainType::PLAIN}, 1},
        {{MovementType::INFANTRY, TerrainType::FOREST}, 2},
        {{MovementType::INFANTRY, TerrainType::MOUNTAIN}, 2},
        {{MovementType::INFANTRY, TerrainType::STREET}, 1},
        {{MovementType::INFANTRY, TerrainType::WATER}, 999},
        

        {{MovementType::TRACKED, TerrainType::PLAIN}, 1},
        {{MovementType::TRACKED, TerrainType::FOREST}, 2},
        {{MovementType::TRACKED, TerrainType::MOUNTAIN}, 999},
        {{MovementType::TRACKED, TerrainType::STREET}, 1},
        {{MovementType::TRACKED, TerrainType::WATER}, 999},
        

        {{MovementType::WHEELED, TerrainType::PLAIN}, 2},
        {{MovementType::WHEELED, TerrainType::FOREST}, 3},
        {{MovementType::WHEELED, TerrainType::MOUNTAIN}, 999},
        {{MovementType::WHEELED, TerrainType::STREET}, 1},
        {{MovementType::WHEELED, TerrainType::WATER}, 999},
        
        {{MovementType::AIR, TerrainType::PLAIN}, 1},
        {{MovementType::AIR, TerrainType::FOREST}, 1},
        {{MovementType::AIR, TerrainType::MOUNTAIN}, 1},
        {{MovementType::AIR, TerrainType::STREET}, 1},
        {{MovementType::AIR, TerrainType::WATER}, 999},

        {{MovementType::SHIPENGINE, TerrainType::PLAIN}, 999},
        {{MovementType::SHIPENGINE, TerrainType::FOREST}, 999},
        {{MovementType::SHIPENGINE, TerrainType::MOUNTAIN}, 999},
        {{MovementType::SHIPENGINE, TerrainType::STREET}, 999},
        {{MovementType::SHIPENGINE, TerrainType::WATER}, 1}      
    };
    
    /*
    The attacker will move towards the defender and thus initiate combat
    @params Takes a reference to the defender

    Will Update the health for both units
    Attacker deals damage to the defender first
    */
    void init_combat(Unit defender); 

    /*
    @params Takes the desired position of the unit and updates its values
    */
    void update_position(int posX, int posY);

    /*
    This function needs to be able to determine the possible movement-paths the unit can take
    MUST take into consideration that different units behave differently on certain terrain
    MUST show all movements possible
    */
    void calculate_movement();
};

#endif // UNIT_HPP