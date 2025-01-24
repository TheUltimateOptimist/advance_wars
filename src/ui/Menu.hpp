#ifndef MENU_SYSTEM_HPP
#define MENU_SYSTEM_HPP

#include <SDL.h>
#include <iostream>
#include <vector>
#include <string>
#include <array>
#include "../scene.hpp"

namespace advanced_wars {

class BaseMenu : public Scene {
public:
    virtual void handleEvent(SDL_Event& event);
    virtual ~BaseMenu();
};

class MainMenu : public Scene {
private:
    size_t selectedOption;
    std::array<std::string, 3> options;
    SDL_Texture* backgroundTexture;
    
public:

    MainMenu(int selectedOption);

    void render(SDL_Renderer *renderer, std::vector<SDL_Event> &events);

    void handleEvent(SDL_Event& event);

    void loadBackground(SDL_Renderer *renderer, const std::string& imagePath);

    ~MainMenu();
};

class PauseMenu : public BaseMenu {
private:
    std::vector<std::string> options;
    int selectedOption;

public:

    void render(SDL_Renderer *renderer, std::vector<SDL_Event> &events);

    void handleEvent(SDL_Event& event);
};

}

#endif // MENU_SYSTEM_HPP