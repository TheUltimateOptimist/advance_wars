#include "Player.hpp"
#include <iostream>

namespace advanced_wars
{

Player::Player(int money, UnitFaction faction)
    : m_money(money), m_alive(true), m_activeTurn(false), m_faction(faction)
{
}

Player::~Player() {}

void Player::startTurn(
    std::unordered_map<int, Unit>& lvUnits, std::unordered_map<int, Building>& lvBuildings)
{
    for (auto& [id, unit] : lvUnits)
    {
        if (unit.getFaction() == m_faction)
        {
            unit.setState(UnitState::IDLE);
        }
    }

    int underControl = 0;

    for (auto& [id, building] : lvBuildings)
    {
        switch (m_faction)
        {
        case UnitFaction::URED:
            if (building.getFaction() == BuildingFaction::URED)
            {
                underControl++;
            }
            break;
        case UnitFaction::UBLUE:
            if (building.getFaction() == BuildingFaction::UBLUE)
            {
                underControl++;
            }
            break;
        case UnitFaction::UYELLOW:
            if (building.getFaction() == BuildingFaction::UYELLOW)
            {
                underControl++;
            }
            break;
        case UnitFaction::UGREEN:
            if (building.getFaction() == BuildingFaction::UGREEN)
            {
                underControl++;
            }
            break;
        case UnitFaction::UPURPLE:
            if (building.getFaction() == BuildingFaction::UPURPLE)
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

void Player::endTurn(std::unordered_map<int, Unit>& lvUnits)
{
    for (auto& [id, unit] : lvUnits)
    {
        if (unit.getFaction() == m_faction)
        {
            unit.setState(UnitState::UNAVAILABLE);
        }
    }
    m_activeTurn = false;
}

UnitFaction Player::getFaction()
{
    return m_faction;
}
} // namespace advanced_wars