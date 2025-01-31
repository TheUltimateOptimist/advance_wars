#include "pausemenu.hpp"
#include "../engine.hpp"
#include <SDL_ttf.h>

namespace advanced_wars
{

PauseMenu::PauseMenu(int selectedOption, SDL_Texture* backgroundTexture)
    : m_selected_option(selectedOption), m_options({"Resume", "Options", "Exit"}),
      m_background_texture(backgroundTexture)
{
    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
    }

    if (!m_background_texture)
    {
        this->m_background_texture = nullptr;
    }
}

PauseMenu::~PauseMenu()
{
    if (m_background_texture)
    {
        SDL_DestroyTexture(m_background_texture);
        m_background_texture = nullptr;
    }
    TTF_Quit();
}

void PauseMenu::render(Engine* engine)
{

    while (!engine->events().empty())
    {
        SDL_Event event = engine->events().at(0);
        engine->events().pop_front();
        handleEvent(engine, event);
    }

    SDL_Renderer* renderer = engine->renderer();

    // Render the existing level
    // engine->render();

    // Render the dialog background
    if (m_background_texture)
    {
        SDL_RenderCopy(renderer, m_background_texture, nullptr, nullptr);
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        return;
    }

    // Render the dialog options on top of the background
    std::string basePath = SDL_GetBasePath();
    std::string relativePath = "assets/ARCADECLASSIC.TTF";
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
            font, m_options[i].c_str(), (i == m_selected_option) ? yellow : white);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect destRect = {100, static_cast<int>(100 + i * 50), textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
    TTF_CloseFont(font);
    SDL_RenderPresent(renderer);
}

void PauseMenu::handleEvent(Engine* engine, SDL_Event& event)
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
        else if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            std::cout << "Resuming game..." << std::endl;
            engine->popScene();
        }
        else if (event.key.keysym.sym == SDLK_RETURN)
        {
            if (m_options[m_selected_option] == "Exit")
            {
                // exit into main menu
                std::cout << "Exiting game..." << std::endl;
                engine->returnToMenu();
            }
            else if (m_options[m_selected_option] == "Resume")
            {
                // resume game
                std::cout << "Resuming game..." << std::endl;
                engine->popScene();
            }
        }
    }
    // Handle events for the pause menu
}

void PauseMenu::loadBackground(SDL_Renderer* renderer, const std::string& imagePath)
{
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface)
    {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return;
    }
    m_background_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

} // namespace advanced_wars