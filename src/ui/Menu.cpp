#include "Menu.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

namespace advanced_wars {

Menu::Menu(int selectedOption)
    : selectedOption(selectedOption),
      options({"Start Game", "Options", "Exit"}), backgroundTexture(nullptr) {
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    std::cerr << "Failed to initialize SDL_image: " << IMG_GetError()
              << std::endl;
  }
}

Menu::~Menu() {
  if (backgroundTexture) {
    SDL_DestroyTexture(backgroundTexture);
  }
  IMG_Quit();
};

void Menu::render(Engine *engine) {

  // Iterate over all events
  while (!engine->events().empty()) {
    SDL_Event event = engine->events().at(0);
    engine->events().pop_front();
    handleEvent(engine, event);
  }

  if (backgroundTexture) {
    SDL_RenderCopy(engine->renderer(), backgroundTexture, nullptr, nullptr);
  } else {
    SDL_SetRenderDrawColor(engine->renderer(), 0, 0, 0, 255);
    SDL_RenderClear(engine->renderer());
  }

  if (TTF_Init() == -1) {
    std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
    return;
  }

  std::string basePath = SDL_GetBasePath();
  std::string relativePath = "assets/ARCADECLASSIC.TTF";
  std::string fullPath = basePath + relativePath;
  TTF_Font *titleFont = TTF_OpenFont(fullPath.c_str(), 48);
  if (!titleFont) {
    std::cerr << "Failed to load title font: " << fullPath << TTF_GetError()
              << std::endl;
    return;
  }

  TTF_Font *menuFont = TTF_OpenFont(fullPath.c_str(), 24);
  if (!menuFont) {
    TTF_CloseFont(titleFont);
    std::cerr << "Failed to load menu font: " << fullPath << TTF_GetError()
              << std::endl;
    return;
  }

  SDL_Color white = {255, 255, 255, 255};
  SDL_Color yellow = {255, 255, 0, 255};

  SDL_Surface *titleSurface =
      TTF_RenderText_Solid(titleFont, "Advanced Wars", white);
  if (titleSurface) {
    SDL_Texture *titleTexture =
        SDL_CreateTextureFromSurface(engine->renderer(), titleSurface);
    SDL_Rect titleRect = {static_cast<int>((800 - titleSurface->w) / 2), 50,
                          titleSurface->w, titleSurface->h};
    SDL_RenderCopy(engine->renderer(), titleTexture, nullptr, &titleRect);
    SDL_DestroyTexture(titleTexture);
    SDL_FreeSurface(titleSurface);
  }

  for (size_t i = 0; i < options.size(); ++i) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(
        menuFont, options[i].c_str(), (i == selectedOption) ? yellow : white);
    if (!textSurface) {
      continue;
    }

    SDL_Texture *textTexture =
        SDL_CreateTextureFromSurface(engine->renderer(), textSurface);
    SDL_Rect textRect = {static_cast<int>((800 - textSurface->w) / 2),
                         static_cast<int>(150 + i * 50), textSurface->w,
                         textSurface->h};
    SDL_RenderCopy(engine->renderer(), textTexture, nullptr, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
  }

  TTF_CloseFont(titleFont);
  TTF_CloseFont(menuFont);
  TTF_Quit();

  SDL_RenderPresent(engine->renderer());
}

void Menu::handleEvent(Engine *engine, SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_DOWN) {
      selectedOption = (selectedOption + 1) % options.size();
    } else if (event.key.keysym.sym == SDLK_UP) {
      selectedOption = (selectedOption - 1 + options.size()) % options.size();
    } else if (event.key.keysym.sym == SDLK_RETURN) {
      if (options[selectedOption] == "Exit") {
        std::cout << "Exiting game..." << std::endl;
        engine->exit();
      }
    }
  }
}

void Menu::loadBackground(SDL_Renderer *renderer,
                          const std::string &imagePath) {
  // Lade das Hintergrundbild
  SDL_Surface *backgroundSurface = IMG_Load(imagePath.c_str());
  if (!backgroundSurface) {
    std::cerr << "Failed to load background image: " << IMG_GetError()
              << std::endl;
    return;
  }

  // Erstelle eine Textur aus der Oberfläche und speichere sie als
  // Klassenmitglied
  backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
  SDL_FreeSurface(backgroundSurface); // Oberfläche freigeben, da sie nicht mehr
                                      // benötigt wird

  if (!backgroundTexture) {
    std::cerr << "Failed to create background texture: " << SDL_GetError()
              << std::endl;
  }
}

} // namespace advanced_wars
