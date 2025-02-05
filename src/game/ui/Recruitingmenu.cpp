#include "Recruitingmenu.hpp"
#include <iostream>
#include <SDL_ttf.h>

namespace advanced_wars
{
    RecruitingMenu::RecruitingMenu() : m_selectedOption(0), unitNames({
    {UnitId::INFANTERY, {"Infantry", 100}},
    {UnitId::MECHANIZED_INFANTERY, {"Bazooka", 200}},
    {UnitId::RECON, {"Recon", 300}},
    {UnitId::APC, {"APC", 400}},
    {UnitId::ARTILLERY, {"Artillery", 500}},
    {UnitId::ANTI_AIR_TANK, {"AA Tank", 600}},
    {UnitId::ANTI_AIR_MISSILE_LAUNCHER, {"Rocket AA", 700}},
    {UnitId::ROCKET_ARTILLERY, {"MLRS", 800}},
    {UnitId::MEDIUM_TANK, {"Medium Tank", 900}},
    {UnitId::NEO_TANK, {"Neo Tank", 1000}},
    {UnitId::HEAVY_TANK, {"Heavy Tank", 1100}},
    {UnitId::LANDER, {"Lander", 1200}},
    {UnitId::CRUISER, {"Cruiser", 1300}},
    {UnitId::SUBMARINE, {"Submarine", 1400}},
    {UnitId::BATTLESHIP, {"Battleship", 1500}},
    {UnitId::TRANSPORT_HELICOPTER, {"Chinook", 1600}},
    {UnitId::BATTLE_HELICOPTER, {"Helicopter", 1700}},
    {UnitId::FIGHTER, {"Fighter", 1800}},
    {UnitId::BOMBER, {"Bomber", 1900}}
    }) {

    }

    void RecruitingMenu::setOptions(const std::vector<UnitId> recruitableUnits) {

        std::vector<std::pair<std::string, int>> options;

        for (UnitId id : recruitableUnits) {
            options.push_back(unitNames.at(id));
            cost2UnitId.insert(std::make_pair(unitNames.at(id).second, id));

        }

        m_options = options;
        m_selectedOption = 0;
    }

    void RecruitingMenu::render(Engine& engine)
{

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
    SDL_Rect box = {m_x, m_y - 3, 150, static_cast<int>(m_options.size() * spacing)};
    SDL_RenderFillRect(engine.renderer(), &box);

    SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 0, 255);
    int i = 0;

    for (auto& [render_name, cost] : m_options)
    {
        //std::pair<std::string, int> unit_option = unitNames.at(cost2UnitId.at(cost));
        if(i == m_selectedOption) {
            m_selectedId = cost2UnitId.at(cost);
        }
        
        SDL_Surface* textSurface = TTF_RenderText_Solid(
            font, render_name.c_str(), (i == m_selectedOption) ? yellow : white);
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
                .at(static_cast<int>(cost2UnitId.at(cost)))
                .at(static_cast<int>(UnitState::IDLE))
                .first;

        SDL_Rect trgt_rect = {
            m_x + 5, 
            m_y + static_cast<int>(i * spacing),                  
            16,                           
            16                                
        };

        SDL_Rect src_rect = {
            5,
            0,
            10,
            10
        };

        SDL_RenderCopy(engine.renderer(), unit_texture, &src_rect, &trgt_rect);

        SDL_Surface* costSurface = TTF_RenderText_Solid(
            font, std::to_string(cost).c_str(), (i == m_selectedOption) ? yellow : white);
        if (!textSurface)
        {
            continue;
        }

        SDL_Texture* costTexture = SDL_CreateTextureFromSurface(engine.renderer(), costSurface);

        SDL_Rect cost_rect {
            m_x + 120 ,
            m_y + static_cast<int>(i * spacing),
            costSurface->w, 
            costSurface->h
        };
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

UnitId RecruitingMenu::getSelectedOption(){
    return m_selectedId;
}

}//namespace advance_wars