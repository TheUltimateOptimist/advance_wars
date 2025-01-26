#include "ContextMenu.hpp"
#include <iostream>
#include <SDL_ttf.h>

namespace advanced_wars {

    ContextMenu::ContextMenu()
        : selectedOption(0) {}

    ContextMenu::~ContextMenu() {}

    void ContextMenu::setOptions(const std::vector<std::string>& newOptions) {
        options = newOptions;
        selectedOption = 0; // Reset auf die erste Option
    }

    void ContextMenu::render(Engine* engine) {
        if (!options.empty()) {
            if (TTF_Init() == -1) {
                std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
                return;
            }

            std::string basePath = SDL_GetBasePath();
            std::string relativePath = "assets/ARCADECLASSIC.TTF";
            std::string fullPath = basePath + relativePath;
            TTF_Font *font = TTF_OpenFont(fullPath.c_str(), 48);
            if (!font) {
                std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
                return;
            }

            SDL_Color white = {255, 255, 255, 255};
            SDL_Color yellow = {255, 255, 0, 255};

            int startY = 100; // Startposition für das Menü
            int spacing = 40; // Abstand zwischen den Optionen

            for (size_t i = 0; i < options.size(); ++i) {
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, options[i].c_str(), (i == selectedOption) ? yellow : white);
                if (!textSurface) {
                    continue;
                }

                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine->renderer(), textSurface);
                SDL_Rect textRect = {200, startY + static_cast<int>(i * spacing), textSurface->w, textSurface->h};
                SDL_RenderCopy(engine->renderer(), textTexture, nullptr, &textRect);

                SDL_DestroyTexture(textTexture);
                SDL_FreeSurface(textSurface);
            }

            TTF_CloseFont(font);
            TTF_Quit();
        }
    }

    void ContextMenu::handleEvent(SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_DOWN) {
                selectedOption = (selectedOption + 1) % options.size();
            } else if (event.key.keysym.sym == SDLK_UP) {
                selectedOption = (selectedOption - 1 + options.size()) % options.size();
            } else if (event.key.keysym.sym == SDLK_RETURN) {
                std::cout << "Selected option: " << options[selectedOption] << std::endl;
            }
        }
    }

}
