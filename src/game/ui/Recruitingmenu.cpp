#include "Recruitingmenu.hpp"
#include <iostream>
#include <SDL_ttf.h>
#include "../Unit.hpp"

namespace advanced_wars
{
    RecruitingMenu::RecruitingMenu() : m_selectedOption(0) {

    }

    void RecruitingMenu::setOptions(const std::vector<std::string>& newOptions) {
        m_options = newOptions;
        m_selectedOption = 0;
        renderableunit = {};
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
    SDL_Rect box = {m_x, m_y - 3, 125, static_cast<int>(m_options.size() * spacing)};
    SDL_RenderFillRect(engine.renderer(), &box);

    SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 0, 255);

    for (size_t i = 0; i < m_options.size(); ++i)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid(
            font, m_options[i].c_str(), (i == m_selectedOption) ? yellow : white);
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
                .at(static_cast<int>(UnitId::INFANTERY))
                .at(static_cast<int>(UnitState::IDLE))
                .first;

        SDL_Rect rect = {
            m_x + 5, // X-Position rechts neben dem Text
            m_y + static_cast<int>(i * spacing),                   // Gleiche Y-Position wie der Text
            16,                            // Breite des Rechtecks
            16                                 // Höhe des Rechtecks entspricht der Texthöhe
        };
        SDL_RenderCopy(engine.renderer(), unit_texture, nullptr, &rect);
        
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
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

std::string RecruitingMenu::getSelectedOption()
{
    return m_options[m_selectedOption];
}

void RecruitingMenu::update(int x, int y)
{
    this->m_x = x;
    this->m_y = y;
}

}//namespace advance_wars