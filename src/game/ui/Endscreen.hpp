#pragma once

#include "../Player.hpp"
#include "../Scene.hpp"

namespace advanced_wars
{
class Endscreen : public Scene
{
    private:
        SDL_Color   m_color;
        int         m_moenyLeft;
        std::string m_playerString;

    public:
        Endscreen(Player& player);
        void render(Engine& engine);

        void handleEvent(Engine& engine, SDL_Event& event);
};

} // namespace advanced_wars
