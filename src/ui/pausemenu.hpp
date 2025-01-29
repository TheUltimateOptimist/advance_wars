#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include "../scene.hpp"
#include <SDL.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace advanced_wars {

class PauseMenu : public Scene {
private:
  size_t selectedOption;
  std::array<std::string, 3> options;
  SDL_Texture *backgroundTexture;

public:
  PauseMenu(int selectedOption, SDL_Texture *backgroundTexture);

  void render(Engine *engine) override;

  void handleEvent(Engine *engine, SDL_Event &event);

  void loadBackground(SDL_Renderer *renderer, const std::string &imagePath);

  ~PauseMenu();
};

} // namespace advanced_wars

#endif // PAUSE_MENU_HPP