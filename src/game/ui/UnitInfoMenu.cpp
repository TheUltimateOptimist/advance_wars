#include "UnitInfoMenu.hpp"
#include <SDL_ttf.h>
#include <iostream>

namespace advanced_wars
{

//UnitInfoMenu::UnitInfoMenu() : m_currentUnit(nullptr), m_isVisible(false) {}

UnitInfoMenu::UnitInfoMenu() : m_currentUnit(nullptr) {
}

std::string UnitInfoMenu::getMovementTypeString(MovementType type) {
    switch (type) {
        case MovementType::FOOT:
            return "Foot";
        case MovementType::WHEELED:
            return "Wheeled";
        case MovementType::TREAD:
            return "Tracked";
        case MovementType::AIR:
            return "Aircraft";
        case MovementType::SEA:
            return "Ship";
        case MovementType::LANDER:
            return "Lander";  
        default:
            return "Unknown";
    }
}

void UnitInfoMenu::handleEvent(Engine& engine, SDL_Event& event)
{
    // Hier kannst du den Code hinzufügen, um die Ereignisse für das UnitInfoMenu zu behandeln
    // Wenn keine spezifische Ereignisbehandlung erforderlich ist, kann diese Methode auch leer
    // bleiben.
}

void UnitInfoMenu::setUnit(Unit& unit)
{
    m_currentUnit = &unit;
    m_isVisible = true;
}

void UnitInfoMenu::render(Engine& engine)
{
    

    // TTF Initialisierung
    if (TTF_Init() == -1)
    {
        std::cerr << "TTF konnte nicht initialisiert werden: " << TTF_GetError() << std::endl;
        return ;
    }

    if (!m_currentUnit || !m_isVisible)
        return;

    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        return;
    }

    std::string basePath = SDL_GetBasePath();
    std::string fullPath = basePath + "res/ARCADECLASSIC.TTF";
    TTF_Font*   font = TTF_OpenFont(fullPath.c_str(), 16);

    if (!font)
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    int       spacing = 5;

    // Draw background box
    SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 255, 255);
    SDL_Rect box = {m_x, m_y - 3, 200 * RENDERING_SCALE, 120 * RENDERING_SCALE};
    SDL_RenderFillRect(engine.renderer(), &box);

    // Render unit sprite
    Spritesheet* spritesheet = engine.getSpritesheet();
    SDL_Texture* unit_texture = spritesheet->getUnitTextures()
                                    .at(static_cast<int>(m_currentUnit->getFaction()))
                                    .at(static_cast<int>(m_currentUnit->getId()))
                                    .at(static_cast<int>(UnitState::IDLE))
                                    .first;

    SDL_Rect sprite_rect = {
        m_x + 10 * RENDERING_SCALE, m_y + 10 * RENDERING_SCALE, 16 * RENDERING_SCALE,
        16 * RENDERING_SCALE};

    SDL_Rect source_rect = {0,0,16,16};
    SDL_RenderCopy(engine.renderer(), unit_texture, &source_rect, &sprite_rect);

    // Render unit information
    std::vector<std::string> info_lines = {
        "HP: " + std::to_string(m_currentUnit->getHealth()),
        "Movement: " + std::to_string(m_currentUnit->m_movementPoints),
        "Ammo: " + std::to_string(m_currentUnit->getAmmo()),
        "Movement Type: " + getMovementTypeString(m_currentUnit->m_movementType),
        "Cost: " + std::to_string(m_currentUnit->m_price)};

    int text_y = m_y + 50 * RENDERING_SCALE;
    for (const auto& line : info_lines)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), white);
        if (!textSurface)
            continue;

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine.renderer(), textSurface);
        SDL_Rect textRect = {m_x + 10 * RENDERING_SCALE, text_y, textSurface->w, textSurface->h};

        SDL_RenderCopy(engine.renderer(), textTexture, nullptr, &textRect);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        text_y += spacing * RENDERING_SCALE;
    }

    TTF_CloseFont(font);
    TTF_Quit();
}

void UnitInfoMenu::update(int x, int y)
{
    m_x = x;
    m_y = y;
}

} // namespace advanced_wars
