#include "Helpmenu.hpp"
#include <SDL_ttf.h>
#include <iostream>

namespace advanced_wars
{

HelpMenu::HelpMenu() {}

void HelpMenu::handleEvent(Engine& engine, SDL_Event& event) {}

std::vector<std::string> helpTable = {
    "Willkommen im Hilfe menu",                                                               // 0
    "Nutzen Sie die PFEILTASTEN zur Navigation in allen Menus",                               // 1
    "Einheiten bewegen; Wenn Sie an der Reihe sind, nutzen Sie die PFEILTASTEN, um den "      // 2
    "Selection_Cursor (das kleine rote Quadrat) auf dem Spielfeld zu verschieben Nutzen Sie " // 3
    "RETURN, um eine Einheit zu selektieren. Nutzen Sie die PFEILTASTEN, um eine Option "     // 4
    "auszuwählen und RETURN, um ihre Auswahl zu bestätigen",                                  // 5
    "Spielregeln;",                                                                           // 6
    "Einheiten dürfen sich in einer Runde nur ein Mal bewegen und ein Mal angreifen",         // 7
    "Gegnerische Einheiten müssen sich in REICHWEITE befinden",                               // 8
    "Einheiten kosten Geld",                                                                  // 9
    "Am Start ihres Zuges erhalten Sie pro Stadt 1000 Geld",                                  // 10
    "Angreifer schießen immer zuerst!",                                                       // 11
    "Achten Sie auf die Einheitenklassen, um Schadensboni auszunutzen!",                      // 12
    "Nutzen Sie diese Werkzeuge, um das Spiel gegen ihren Gegner zu gewinnen!"};              // 13

void HelpMenu::render(advanced_wars::Engine& engine)
{
    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
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
    int       spacing = 25;
    int       boxWidth = 600;
    int       boxHeight = static_cast<int>(helpTable.size() * spacing + 20);

    SDL_SetRenderDrawColor(engine.renderer(), 0, 0, 255, 255);
    SDL_Rect box = {50, 50, boxWidth, boxHeight};
    SDL_RenderFillRect(engine.renderer(), &box);

    int text_x = 60;
    int text_y = 60;

    renderTextPortion(engine, helpTable[0], font, white, boxWidth, text_x, text_y);
    text_y += spacing;
    std::string text = helpTable[1] + helpTable[2] + helpTable[3] + helpTable[4] + helpTable[5];
    renderTextPortion(engine, text, font, white, boxWidth, text_x, text_y);
    text_y += spacing;
    // forgive me I have to do this because of styleguide fuckery
    text = helpTable[6] + helpTable[7] + helpTable[8] + helpTable[9] + helpTable[10];
    std::string textPartTwo = helpTable[11] + helpTable[12] + helpTable[13];
    text = text.append(textPartTwo);
    renderTextPortion(engine, text, font, white, boxWidth, text_x, text_y);
    text_y += spacing;

    TTF_CloseFont(font);
    TTF_Quit();
}

void HelpMenu::renderTextPortion(
    Engine& engine, std::string text, TTF_Font* font, SDL_Color color, int boxWidth, int text_x,
    int text_y)
{
    SDL_Surface* textSurface =
        TTF_RenderUTF8_Solid_Wrapped(font, text.c_str(), color, boxWidth - 5);
    if (!textSurface)
        return;

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine.renderer(), textSurface);
    SDL_Rect     textRect = {text_x, text_y, textSurface->w, textSurface->h};
    SDL_RenderCopy(engine.renderer(), textTexture, nullptr, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

} // namespace advanced_wars