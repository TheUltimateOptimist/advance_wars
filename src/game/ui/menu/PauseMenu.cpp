#include "PauseMenu.hpp"
#include "../../core/Engine.hpp"

#include <SDL_ttf.h>
#include <iostream>

namespace advanced_wars
{

PauseMenu::PauseMenu(int selectedOption, SDL_Texture* backgroundTexture)
    : m_selectedOption(selectedOption), m_options({"Resume", "Options", "Exit"}),
      m_backgroundTexture(backgroundTexture)
{
    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
    }

    if (!m_backgroundTexture)
    {
        this->m_backgroundTexture = nullptr;
    }
}

PauseMenu::~PauseMenu()
{
    if (m_backgroundTexture)
    {
        SDL_DestroyTexture(m_backgroundTexture);
        m_backgroundTexture = nullptr;
    }
    TTF_Quit();
}

void PauseMenu::render(Engine& engine)
{
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
    }

    SDL_Renderer* renderer = engine.renderer();

    // Render the existing level
    // engine->render();

    // Render the dialog background
    if (m_backgroundTexture)
    {
        SDL_RenderCopy(renderer, m_backgroundTexture, nullptr, nullptr);
    }

    // Render the dialog options on top of the background
    std::string basePath = SDL_GetBasePath();
    std::string relativePath = "res/ARCADECLASSIC.TTF";
    std::string fullPath = basePath + relativePath;

    TTF_Font* font = TTF_OpenFont(fullPath.c_str(), 24);
    if (!font)
    {
        std::cerr << "Failed to load menu font: " << fullPath << " " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    for (size_t i = 0; i < m_options.size(); ++i)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid(
            font, m_options[i].c_str(), (i == m_selectedOption) ? yellow : white);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect destRect = {100, static_cast<int>(100 + i * 50), textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
    TTF_CloseFont(font);
    TTF_Quit();
}

void PauseMenu::handleEvent(Engine& engine, SDL_Event& event)
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
        else if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            std::cout << "Resuming game..." << std::endl;
            engine.popScene();
        }
        else if (event.key.keysym.sym == SDLK_RETURN)
        {
            if (m_options[m_selectedOption] == "Exit")
            {
                // exit into main menu
                std::cout << "Exiting game..." << std::endl;
                engine.returnToMenu();
            }
            else if (m_options[m_selectedOption] == "Resume")
            {
                // resume game
                std::cout << "Resuming game..." << std::endl;
                engine.popScene();
            }
        }
    }
    // Handle events for the pause menu
}

void PauseMenu::loadBackground(Engine& engine, const std::string& imagePath)
{
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface)
    {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    m_backgroundTexture = SDL_CreateTextureFromSurface(engine.renderer(), surface);
    SDL_FreeSurface(surface);
}

} // namespace advanced_wars
