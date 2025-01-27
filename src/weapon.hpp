#pragma once

#include "unit.hpp"
#include <unordered_map>

namespace advanced_wars {

// Forward Declaration
enum class UnitId;

class Weapon {
public:
  // Ranges
  int min_range;
  int max_range;

  // Damage
  std::unordered_map<UnitId, int> damage;
};

} // namespace advanced_wars