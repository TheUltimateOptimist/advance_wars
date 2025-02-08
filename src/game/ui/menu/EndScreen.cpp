#include "EndScreen.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

namespace advanced_wars
{
Endscreen::Endscreen(Player& player) : m_moenyLeft(player.getMoney())
{
    std::cout << "Player faction: " << static_cast<int>(player.getFaction()) << std::endl;
    switch (player.getFaction())
    {
    case UnitFaction::UBLUE:
        m_color = {0, 0, 255, 255};
        m_playerString = "Blue";
        break;
    case UnitFaction::UGREEN:
        m_color = {0, 255, 0, 255};
        m_playerString = "Green";
        break;
    case UnitFaction::UPURPLE:
        m_color = {255, 0, 255, 255};
        m_playerString = "Purple";
        break;
    case UnitFaction::URED:
        m_color = {255, 0, 0, 255};
        m_playerString = "Red";
        break;
    case UnitFaction::UYELLOW:
        m_color = {255, 255, 0, 255};
        m_playerString = "Yellow";
        break;
    default:
        break;
    }
}

void Endscreen::render(Engine& engine)
{
    std::string basePath = SDL_GetBasePath();
    SDL_Color   white = {255, 255, 255, 255};
    SDL_Color   yellow = {255, 255, 0, 255};

    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        return;
    }
    // Draw Background
    std::string relativePath = "res/main_background.png";
    std::string fullPath = SDL_GetBasePath() + relativePath;

    SDL_Surface* backgroundSurface = IMG_Load(fullPath.c_str());
    if (!backgroundSurface)
    {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
        return;
    }

    SDL_Texture* backgroundTexture =
        SDL_CreateTextureFromSurface(engine.renderer(), backgroundSurface);

    SDL_RenderCopy(engine.renderer(), backgroundTexture, nullptr, nullptr);

    // Draw Foreground
    relativePath = "res/ARCADECLASSIC.TTF";
    fullPath = basePath + relativePath;
    TTF_Font* titleFont = TTF_OpenFont(fullPath.c_str(), 48);

    std::string  titleText = "Player " + m_playerString + " won!";
    SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, titleText.c_str(), m_color);

    if (titleSurface)
    {
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(engine.renderer(), titleSurface);
        SDL_Rect     titleRect = {
            static_cast<int>((960 - titleSurface->w) / 2), 50, titleSurface->w, titleSurface->h};
        SDL_RenderCopy(engine.renderer(), titleTexture, nullptr, &titleRect);
        SDL_DestroyTexture(titleTexture);
        SDL_FreeSurface(titleSurface);
    }

    TTF_Font* menuFont = TTF_OpenFont(fullPath.c_str(), 24);

    std::string  moneyText = "Money left*    " + std::to_string(m_moenyLeft);
    SDL_Surface* statsSurface = TTF_RenderText_Solid(menuFont, moneyText.c_str(), white);

    if (statsSurface)
    {
        SDL_Texture* statsTexture = SDL_CreateTextureFromSurface(engine.renderer(), statsSurface);
        SDL_Rect     titleRect = {
            static_cast<int>((800 - statsSurface->w) / 2), 150, statsSurface->w, statsSurface->h};
        SDL_RenderCopy(engine.renderer(), statsTexture, nullptr, &titleRect);
        SDL_DestroyTexture(statsTexture);
        SDL_FreeSurface(statsSurface);
    }

    SDL_Surface* optionSurface = TTF_RenderText_Solid(menuFont, "Back to Menu", yellow);

    if (optionSurface)
    {
        SDL_Texture* optionTexture = SDL_CreateTextureFromSurface(engine.renderer(), optionSurface);
        SDL_Rect     titleRect = {
            static_cast<int>((960 - optionSurface->w) / 2), 400, optionSurface->w,
            optionSurface->h};
        SDL_RenderCopy(engine.renderer(), optionTexture, nullptr, &titleRect);
        SDL_DestroyTexture(optionTexture);
        SDL_FreeSurface(optionSurface);
    }

    // Free resources
    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyTexture(backgroundTexture);
}

void Endscreen::handleEvent(Engine& engine, SDL_Event& event)
{
    if (event.key.keysym.sym == SDLK_RETURN)
    {
        engine.returnToMenu();
    }
}
} // namespace advanced_wars
