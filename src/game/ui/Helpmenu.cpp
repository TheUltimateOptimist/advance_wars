#include "Helpmenu.hpp"
#include <SDL_ttf.h>
#include <iostream>

namespace advanced_wars {

    HelpMenu::HelpMenu() {

    }

    void HelpMenu::handleEvent(Engine& engine, SDL_Event& event) {

    }

    std::vector<std::string> helpTable = {"Willkommen im Hilfemenu", "Nutzen Sie die PFEILTASTEN zur Navigation in diesem Menu",
    "Einheiten bewegen:", "Wenn Sie an der Reihe sind, nutzen Sie die PFEILTASTEN, um den Selection_Cursor (das kleine rote Quadrat) auf dem Spielfeld zu verschieben",
    "Nutzen Sie RETURN, um eine Einheit zu selektieren.", "Nutzen Sie die PFEILTASTEN, um eine Option auszuwählen und RETURN, um ihre Auswahl zu bestätigen",
    "Spielregeln:", "Einheiten dürfen sich in einer Runde nur ein Mal bewegen und ein Mal angreifen", "Gegnerische Einheiten müssen sich in REICHWEITE befinden",
    "Einheiten kosten Geld", "Am Start ihres Zuges erhalten Sie pro Stadt 1000 Geld", "Angreifer schießen immer zuerst!", "Achten Sie auf die Einheitenklassen, um Schadensboni auszunutzen!",
    "Nutzen Sie diese Werkzeuge, um das Spiel gegen ihren Gegner zu gewinnen!"};

void HelpMenu::render(advanced_wars::Engine& engine) {
    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        return;
    }

    std::string basePath = SDL_GetBasePath();
    std::string relativePath = "res/ARCADECLASSIC.TTF";
    std::string fullPath = basePath + relativePath;
    TTF_Font* font = TTF_OpenFont(fullPath.c_str(), 16);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    int spacing = 25; 
    int boxWidth = 600;
    int boxHeight = static_cast<int>(helpTable.size() * spacing + 20);

    SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 255, 255);
    SDL_Rect box = {50, 50, boxWidth, boxHeight};
    SDL_RenderFillRect(engine.renderer(), &box);

    int text_x = 60; 
    int text_y = 60;
    
    for (const auto& line : helpTable) {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), white);
        if (!textSurface) continue;

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine.renderer(), textSurface);
        SDL_Rect textRect = {text_x, text_y, textSurface->w, textSurface->h};
        SDL_RenderCopy(engine.renderer(), textTexture, nullptr, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        text_y += spacing;
    }

    TTF_CloseFont(font);
    TTF_Quit();
}

}