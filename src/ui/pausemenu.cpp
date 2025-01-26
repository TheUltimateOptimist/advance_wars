#include "pausemenu.hpp"
#include "../engine.hpp"
#include <SDL_ttf.h>

namespace advanced_wars {

PauseMenu::PauseMenu(int selectedOption, SDL_Texture *backgroundTexture)
    : selectedOption(selectedOption),
      options({"Resume", "Options", "Exit"}), backgroundTexture(backgroundTexture) {
  // Initialize SDL_ttf
  if (TTF_Init() == -1) {
    std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
  }
}

PauseMenu::~PauseMenu() {
  if (backgroundTexture) {
    SDL_DestroyTexture(backgroundTexture);
  }
  TTF_Quit();
}

void PauseMenu::render(Engine *engine) {
  SDL_Renderer *renderer = engine->renderer();

  // Render the existing level
  //engine->render();

  // Render the dialog background
  if (backgroundTexture) {
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
  }

  if (TTF_Init() == -1) {
    std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
    return;
  }

  // Render the dialog options on top of the background
  std::string basePath = SDL_GetBasePath();
  std::string relativePath = "assets/ARCADECLASSIC.TTF";
  std::string fullPath = basePath + relativePath;

  TTF_Font *font = TTF_OpenFont(fullPath.c_str(), 24);
  if (!font) {
    std::cerr << "Failed to load menu font: " << fullPath << " " << TTF_GetError()
              << std::endl;
    return;
  }

  SDL_Color white = {255, 255, 255, 255};
  SDL_Color yellow = {255, 255, 0, 255};

  for (size_t i = 0; i < options.size(); ++i) {
    SDL_Surface *textSurface = TTF_RenderText_Solid(
        font, options[i].c_str(), (i == selectedOption) ? yellow : white);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    SDL_Rect destRect = {100, static_cast<int>(100 + i * 50), textSurface->w, textSurface->h};  
    SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
  }
  TTF_CloseFont(font);
  SDL_RenderPresent(renderer);
}

void PauseMenu::handleEvent(Engine *engine, SDL_Event &event) {
  // Handle events for the pause menu
}



void PauseMenu::loadBackground(SDL_Renderer *renderer, const std::string &imagePath) {
  SDL_Surface *surface = IMG_Load(imagePath.c_str());
  if (!surface) {
    std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
    return;
  }
  backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
}

} // namespace advanced_wars