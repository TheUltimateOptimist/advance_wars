#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include "Menu.hpp"

namespace advanced_wars 
{

    MainMenu::MainMenu(int selectedOption)
        : selectedOption(selectedOption), 
        options({"Start Game", "Options", "Exit"}) 
    {
        
    }

    MainMenu::~MainMenu() {
        
    };

    void MainMenu::render(SDL_Renderer *renderer, std::vector<SDL_Event> &events) {

        if (events.size() > 0) {
            for (auto event: events) {
                handleEvent(event);
            }
        }
        

        // Clear the screen with a background color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Initialize SDL_TTF if not already done
        if (TTF_Init() == -1) {
            std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
            return;
        }

        // Load a font
        TTF_Font* font = TTF_OpenFont("/usr/share/fonts/ARCADECLASSIC.TTF", 24);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            return;
        }

        // Colors for text rendering
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color yellow = {255, 255, 0, 255};

        // Render each menu option
        for (size_t i = 0; i < options.size(); ++i) {
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, options[i].c_str(), (i == selectedOption) ? yellow : white);
            if (!textSurface) {
                std::cerr << "Failed to create text surface: " << TTF_GetError() << std::endl;
                continue;
            }

            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (!textTexture) {
                std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
                SDL_FreeSurface(textSurface);
                continue;
            }

            // Define text position and size
            SDL_Rect textRect = {100, static_cast<int>(100 + i * 50), textSurface->w, textSurface->h};

            // Render the texture
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            // Clean up resources
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
        }

        // Close the font and quit TTF
        TTF_CloseFont(font);
        TTF_Quit();

        // Present the rendered content
        SDL_RenderPresent(renderer);
    }

    void MainMenu::handleEvent(SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_DOWN) {
                selectedOption = (selectedOption + 1) % options.size();
            } else if (event.key.keysym.sym == SDLK_UP) {
                selectedOption = (selectedOption - 1 + options.size()) % options.size();
            } else if (event.key.keysym.sym == SDLK_RETURN) {
                if (options[selectedOption] == "Exit") {
                    std::cout << "Exiting game..." << std::endl;
                    // Exit logic here
                }
            }
        }
    }

}
