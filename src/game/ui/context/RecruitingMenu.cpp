#include "RecruitingMenu.hpp"

#include <SDL_ttf.h>
#include <iostream>

namespace advanced_wars
{
RecruitingMenu::RecruitingMenu()
    : m_selectedOption(0), unitNames({
                               {                UnitId::INFANTERY,    "Infantry"},
                               {     UnitId::MECHANIZED_INFANTERY,     "Bazooka"},
                               {                    UnitId::RECON,       "Recon"},
                               {                      UnitId::APC,         "APC"},
                               {                UnitId::ARTILLERY,   "Artillery"},
                               {            UnitId::ANTI_AIR_TANK,     "AA Tank"},
                               {UnitId::ANTI_AIR_MISSILE_LAUNCHER,   "Rocket AA"},
                               {         UnitId::ROCKET_ARTILLERY,        "MLRS"},
                               {              UnitId::MEDIUM_TANK, "Medium Tank"},
                               {                 UnitId::NEO_TANK,    "Neo Tank"},
                               {               UnitId::HEAVY_TANK,  "Heavy Tank"},
                               {                   UnitId::LANDER,      "Lander"},
                               {                  UnitId::CRUISER,     "Cruiser"},
                               {                UnitId::SUBMARINE,   "Submarine"},
                               {               UnitId::BATTLESHIP,  "Battleship"},
                               {     UnitId::TRANSPORT_HELICOPTER,     "Chinook"},
                               {        UnitId::BATTLE_HELICOPTER,  "Helicopter"},
                               {                  UnitId::FIGHTER,     "Fighter"},
                               {                   UnitId::BOMBER,      "Bomber"}
})
{
}

void RecruitingMenu::setOptions(const std::vector<UnitId> recruitableUnits)
{

    m_options = recruitableUnits;
    m_selectedOption = 0;
}

void RecruitingMenu::render(Engine& engine)
{
    Config&      config = engine.getUnitConfig();
    Spritesheet* spritesheet = engine.getSpritesheet();

    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        return;
    }

    if (m_options.empty())
    {
        // TODO handle somehow
        return;
    }

    std::string basePath = SDL_GetBasePath();
    std::string relativePath = "res/ARCADECLASSIC.TTF";
    std::string fullPath = basePath + relativePath;
    TTF_Font*   font = TTF_OpenFont(fullPath.c_str(), 16);
    if (!font)
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {192, 255, 0, 255};

    int spacing = 20; // Abstand zwischen den Optionen
    // box around options
    SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 255, 255);
    SDL_Rect box = {m_x, m_y - 3, 175, static_cast<int>(m_options.size() * spacing)};
    SDL_RenderFillRect(engine.renderer(), &box);

    SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 0, 255);
    int i = 0;

    for (UnitId id : m_options)
    {
        // std::pair<std::string, int> unit_option = unitNames.at(cost2UnitId.at(cost));
        if (i == m_selectedOption)
        {
            m_selectedId = id;
        }

        SDL_Surface* textSurface = TTF_RenderText_Solid(
            font, unitNames.at(id).c_str(), (i == m_selectedOption) ? yellow : white);
        if (!textSurface)
        {
            continue;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine.renderer(), textSurface);
        SDL_Rect     textRect = {
            m_x + 10 + 16, m_y + static_cast<int>(i * spacing), textSurface->w, textSurface->h};
        SDL_RenderCopy(engine.renderer(), textTexture, nullptr, &textRect);

        SDL_Texture* unit_texture = spritesheet->getUnitTextures()
                                        .at(static_cast<int>(UnitFaction::URED))
                                        .at(static_cast<int>(id))
                                        .at(static_cast<int>(UnitState::IDLE))
                                        .first;

        SDL_Rect trgt_rect = {m_x + 5, m_y + static_cast<int>(i * spacing), 16, 16};

        SDL_Rect src_rect = {5, 0, 10, 10};

        SDL_RenderCopy(engine.renderer(), unit_texture, &src_rect, &trgt_rect);

        SDL_Surface* costSurface = TTF_RenderText_Solid(
            font, std::to_string(config.getUnitCost(id)).c_str(),
            (i == m_selectedOption) ? yellow : white);
        if (!textSurface)
        {
            continue;
        }

        SDL_Texture* costTexture = SDL_CreateTextureFromSurface(engine.renderer(), costSurface);

        SDL_Rect cost_rect{
            m_x + 120, m_y + static_cast<int>(i * spacing), costSurface->w, costSurface->h};
        SDL_RenderCopy(engine.renderer(), costTexture, nullptr, &cost_rect);

        SDL_DestroyTexture(costTexture);
        SDL_FreeSurface(costSurface);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
        i++;
    }

    TTF_CloseFont(font);
    TTF_Quit();
}

void RecruitingMenu::handleEvent(Engine& engine, SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_DOWN)
        {
            m_selectedOption = (m_selectedOption + 1) % m_options.size();
        }
        else if (event.key.keysym.sym == SDLK_UP)
        {
            m_selectedOption = (m_selectedOption - 1 + m_options.size()) % m_options.size();
        }
    }
}

void RecruitingMenu::update(int x, int y)
{
    this->m_x = x;
    this->m_y = y;
}

UnitId RecruitingMenu::getSelectedOption()
{
    return m_selectedId;
}

} // namespace advanced_wars
