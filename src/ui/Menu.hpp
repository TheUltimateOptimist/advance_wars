#ifndef MENU_SYSTEM_HPP
#define MENU_SYSTEM_HPP

#include "../scene.hpp"
#include <SDL.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace advanced_wars {

class Menu : public Scene {
private:
  size_t selectedOption;
  std::array<std::string, 3> options;
  SDL_Texture *backgroundTexture;

public:
  Menu(int selectedOption);

  void render(Engine *engine);

  void handleEvent(Engine *engine, SDL_Event &event);

  void loadBackground(SDL_Renderer *renderer, const std::string &imagePath);

  ~Menu();
};

} // namespace advanced_wars

#endif // MENU_SYSTEM_HPP