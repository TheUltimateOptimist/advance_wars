#include "Player.hpp"

namespace advanced_wars
{

Player::Player() {}

Player::~Player() {}

void Player::startTurn()
{
    // TODO reset availability of all allied units (all others are unavailable)
    // TODO add calculated money to this player
    // TODO set this player as the current active player
}

void Player::endTurn()
{
    // TODO set all allied units as unavailable
    // TODO set this player as inactive
}

} // namespace advanced_wars