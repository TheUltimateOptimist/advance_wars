#ifndef MENU_SYSTEM_HPP
#define MENU_SYSTEM_HPP

#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "../scene.hpp"

namespace advanced_wars {

class Menu : public Scene {
private:
    size_t selectedOption;
    std::array<std::string, 3> options;
    SDL_Texture* backgroundTexture;
    
public:

    Menu(int selectedOption);

    void render(SDL_Renderer *renderer, std::vector<SDL_Event> &events);

    void handleEvent(SDL_Event& event);

    void loadBackground(SDL_Renderer *renderer, const std::string& imagePath);

    ~Menu();
};

}

#endif // MENU_SYSTEM_HPP