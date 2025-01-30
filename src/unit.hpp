#pragma once

#include <unordered_map>
#include "engine.hpp"
#include "weapon.hpp"
#include <optional>

namespace advanced_wars {

enum class UnitFaction {
  URED = 0,
  UBLUE = 1,
  UGREEN = 2,
  UYELLOW = 3,
  UPURPLE = 4,
};

enum class UnitId {
  INFANTERY = 0,
  MECHANIZED_INFANTERY = 1,
  RECON = 2,
  MEDIUM_TANK = 3,
  HEAVY_TANK = 4,
  NEO_TANK = 5,
  APC = 6,
  ANTI_AIR_TANK = 7,
  ARTILLERY = 8,
  ROCKET_ARTILLERY = 9,
  ANTI_AIR_MISSILE_LAUNCHER = 10,
  FIGHTER = 11,
  BOMBER = 12,
  BATTLE_HELICOPTER = 13,
  TRANSPORT_HELICOPTER = 14,
  BATTLESHIP = 15,
  CRUISER = 16,
  LANDER = 17,
  SUBMARINE = 18,
};

enum class UnitState {
  IDLE = 0,
  UNAVAILABLE = 1,
  MOVEMENTLEFT = 2,
  MOVEMENTRIGHT = 3,
  MOVEMENTDOWN = 4,
  MOVEMENTUP = 5,
};

enum class MovementType {
  FOOT = 0,
  TIRES = 1,
  TREAD = 2,
  AIR = 3,
  SHIP = 4,
  LANDER = 5,
};

using MatchupTable = std::unordered_map<UnitId, std::unordered_map<UnitId, int>>;

class Unit {
public:
  int x;
  int y;
  int health; //health equals max_health at construction


  Unit(int x, int y, UnitFaction faction, UnitId id, UnitState state);
  ~Unit() {
        //Assuming that the destruktion of a unit triggers events
    }

  void render(Engine &engine, int scale);

  /*
  Check if attacker is in Range to initiate combat
  TODO: This should probably tie back into rendering the units differently
  If a unit is selected, it should call inRange on all other enemy units on the field
  */

 bool inRange(Unit *enemy);

  /*
  The attacker will move towards the defender and thus initiate combat
  @params Takes a reference to the defender

  Will Update the health for both units
  Attacker deals damage to the defender first
  */

  void attack(Unit *enemy);


  /*
  @params Takes the desired position of the unit and updates its values
  This will teleport the unit, there is no smooth transition between tiles
  */
  void update_position(int posX, int posY);

  /*
  This function needs to be able to determine the possible movement-paths the unit can take
  MUST take into consideration that different units behave differently on certain terrain
  MUST show all movements possible
  */
  void calculate_movement();


  /*
  This function fills the MatchupTable
  It would be better if this table would be placed in the level
  */
 MatchupTable fill_matchupTable(int);

/*
This function will be called by an external event-handler, eventually.
Currently, it should be called if a Unit is interacted with and the resulting SDL_EVENT is passed through, and then decided upon
*/
void onClick(SDL_Event event, std::vector<Unit> &unitVector);

private:
  UnitFaction faction;
  UnitId id;
  UnitState state;

 
  int max_health; // max_health required for damage_scaling
  int range;
  int fuel;
  int max_fuel;

  bool has_moved;
  bool has_attacked;
  bool is_selected;
  bool is_targeted;
  Weapon secondary_weapon;
  Weapon primary_weapon;

  int ammo;
 
};

} // namespace advanced_wars