#include "contextmenu.hpp"
#include <SDL_ttf.h>
#include <iostream>

namespace advanced_wars
{

ContextMenu::ContextMenu() : m_selected_option(0) {}

ContextMenu::~ContextMenu() {}

void ContextMenu::setOptions(const std::vector<std::string>& newOptions)
{
    m_options = newOptions;
    m_selected_option = 0; // Reset auf die erste Option
}

void ContextMenu::render(Engine* engine)
{
    if (!m_options.empty())
    {
        if (TTF_Init() == -1)
        {
            std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
            return;
        }

        std::string basePath = SDL_GetBasePath();
        std::string relativePath = "assets/ARCADECLASSIC.TTF";
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
        SDL_SetRenderDrawColor(engine->renderer(), 0, 0, 255, 255);
        SDL_Rect box = {m_x, m_y - 3, 50, static_cast<int>(m_options.size() * spacing)};
        SDL_RenderFillRect(engine->renderer(), &box);

        SDL_SetRenderDrawColor(engine->renderer(), 0, 0, 0, 255);

        for (size_t i = 0; i < m_options.size(); ++i)
        {
            SDL_Surface* textSurface = TTF_RenderText_Solid(
                font, m_options[i].c_str(), (i == m_selected_option) ? yellow : white);
            if (!textSurface)
            {
                continue;
            }

            SDL_Texture* textTexture =
                SDL_CreateTextureFromSurface(engine->renderer(), textSurface);
            SDL_Rect textRect = {
                m_x + 10, m_y + static_cast<int>(i * spacing), textSurface->w, textSurface->h};
            SDL_RenderCopy(engine->renderer(), textTexture, nullptr, &textRect);

            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }

        TTF_CloseFont(font);
        TTF_Quit();
    }
}

void ContextMenu::handleEvent(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_DOWN)
        {
            m_selected_option = (m_selected_option + 1) % m_options.size();
        }
        else if (event.key.keysym.sym == SDLK_UP)
        {
            m_selected_option = (m_selected_option - 1 + m_options.size()) % m_options.size();
        }
    }
}

std::string ContextMenu::getSelectedOption()
{
    return m_options[m_selected_option];
}

void ContextMenu::update(int x, int y)
{
    this->m_x = x;
    this->m_y = y;
}

} // namespace advanced_wars
