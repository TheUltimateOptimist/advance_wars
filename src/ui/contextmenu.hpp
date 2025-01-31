#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include "../scene.hpp"
#include "../engine.hpp"

namespace advanced_wars {

class ContextMenu : public Scene {
private:
    size_t selectedOption;
    std::vector<std::string> options;
    int x;
    int y;

public:
    ContextMenu();    

    void setOptions(const std::vector<std::string>& newOptions);

    void render(Engine* engine) override;

    void handleEvent(SDL_Event& event);

    void update(int x, int y);

    std::string getSelectedOption();

    ~ContextMenu();
};

}