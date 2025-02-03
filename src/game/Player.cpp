#include "Player.hpp"
#include <iostream>

namespace advanced_wars
{

Player::Player(int money, PlayerFaction faction) : m_money(money), m_faction(faction) {}

Player::~Player() {}

void Player::startTurn(
    std::unordered_map<int, Unit> lvUnits, std::unordered_map<int, Building> lvBuildings)
{
    for (auto& [id, unit] : lvUnits)
    {
        switch (m_faction)
        {
        case PlayerFaction::RED:
            if (unit.getFaction() == UnitFaction::URED)
            {
                unit.setState(UnitState::IDLE);
            }
            break;
        case PlayerFaction::BLUE:
            if (unit.getFaction() == UnitFaction::UBLUE)
            {
                unit.setState(UnitState::IDLE);
            }
            break;
        case PlayerFaction::YELLOW:
            if (unit.getFaction() == UnitFaction::UYELLOW)
            {
                unit.setState(UnitState::IDLE);
            }
            break;
        case PlayerFaction::GREEN:
            if (unit.getFaction() == UnitFaction::UGREEN)
            {
                unit.setState(UnitState::IDLE);
            }
            break;
        case PlayerFaction::PURPLE:
            if (unit.getFaction() == UnitFaction::UPURPLE)
            {
                unit.setState(UnitState::IDLE);
            }
            break;

        default:
            break;
        }
    }

    int underControl = 0;

    for (auto& [id, building] : lvBuildings)
    {
        switch (m_faction)
        {
        case PlayerFaction::RED:
            if (building.getFaction() == BuildingFaction::RED)
            {
                underControl++;
            }
            break;
        case PlayerFaction::BLUE:
            if (building.getFaction() == BuildingFaction::BLUE)
            {
                underControl++;
            }
            break;
        case PlayerFaction::YELLOW:
            if (building.getFaction() == BuildingFaction::YELLOW)
            {
                underControl++;
            }
            break;
        case PlayerFaction::GREEN:
            if (building.getFaction() == BuildingFaction::GREEN)
            {
                underControl++;
            }
            break;
        case PlayerFaction::PURPLE:
            if (building.getFaction() == BuildingFaction::PURPLE)
            {
                underControl++;
            }
            break;

        default:
            break;
        }
    }

    m_money += 1000 * underControl;

    m_activeTurn = true;
}

void Player::endTurn(std::unordered_map<int, Unit> lvUnits)
{
    for (auto& [id, unit] : lvUnits)
    {
        switch (m_faction)
        {
        case PlayerFaction::RED:
            if (unit.getFaction() == UnitFaction::URED)
            {
                unit.setState(UnitState::UNAVAILABLE);
            }
            break;
        case PlayerFaction::BLUE:
            if (unit.getFaction() == UnitFaction::UBLUE)
            {
                unit.setState(UnitState::UNAVAILABLE);
            }
            break;
        case PlayerFaction::YELLOW:
            if (unit.getFaction() == UnitFaction::UYELLOW)
            {
                unit.setState(UnitState::UNAVAILABLE);
            }
            break;
        case PlayerFaction::GREEN:
            if (unit.getFaction() == UnitFaction::UGREEN)
            {
                unit.setState(UnitState::UNAVAILABLE);
            }
            break;
        case PlayerFaction::PURPLE:
            if (unit.getFaction() == UnitFaction::UPURPLE)
            {
                unit.setState(UnitState::UNAVAILABLE);
            }
            break;

        default:
            break;
        }
    }
    m_activeTurn = false;
}
} // namespace advanced_wars