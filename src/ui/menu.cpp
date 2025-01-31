#include "menu.hpp"
#include "../building.hpp"
#include "../level.hpp"
#include "../spritesheet.hpp"
#include "../tile.hpp"
#include "../unit.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

namespace advanced_wars
{

Menu::Menu(int selectedOption)
    : m_selected_option(selectedOption), m_options({"Start Game", "Options", "Exit"}),
      m_background_texture(nullptr)
{
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
    }
}

Menu::~Menu()
{
    if (m_background_texture)
    {
        SDL_DestroyTexture(m_background_texture);
    }
    IMG_Quit();
};

void Menu::render(Engine* engine)
{

    // Iterate over all events
    while (!engine->events().empty())
    {
        SDL_Event event = engine->events().at(0);
        engine->events().pop_front();
        handleEvent(engine, event);
    }

    if (m_background_texture)
    {
        SDL_RenderCopy(engine->renderer(), m_background_texture, nullptr, nullptr);
    }
    else
    {
        SDL_SetRenderDrawColor(engine->renderer(), 0, 0, 0, 255);
        SDL_RenderClear(engine->renderer());
    }

    if (TTF_Init() == -1)
    {
        std::cerr << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
        return;
    }

    std::string basePath = SDL_GetBasePath();
    std::string relativePath = "assets/ARCADECLASSIC.TTF";
    std::string fullPath = basePath + relativePath;
    TTF_Font*   titleFont = TTF_OpenFont(fullPath.c_str(), 48);
    if (!titleFont)
    {
        std::cerr << "Failed to load title font: " << fullPath << TTF_GetError() << std::endl;
        return;
    }

    TTF_Font* menuFont = TTF_OpenFont(fullPath.c_str(), 24);
    if (!menuFont)
    {
        TTF_CloseFont(titleFont);
        std::cerr << "Failed to load menu font: " << fullPath << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, "Advanced Wars", white);
    if (titleSurface)
    {
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(engine->renderer(), titleSurface);
        SDL_Rect     titleRect = {
            static_cast<int>((800 - titleSurface->w) / 2), 50, titleSurface->w, titleSurface->h};
        SDL_RenderCopy(engine->renderer(), titleTexture, nullptr, &titleRect);
        SDL_DestroyTexture(titleTexture);
        SDL_FreeSurface(titleSurface);
    }

    for (size_t i = 0; i < m_options.size(); ++i)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid(
            menuFont, m_options[i].c_str(), (i == m_selected_option) ? yellow : white);
        if (!textSurface)
        {
            continue;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine->renderer(), textSurface);
        SDL_Rect     textRect = {
            static_cast<int>((800 - textSurface->w) / 2), static_cast<int>(150 + i * 50),
            textSurface->w, textSurface->h};
        SDL_RenderCopy(engine->renderer(), textTexture, nullptr, &textRect);

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
    }

    TTF_CloseFont(titleFont);
    TTF_CloseFont(menuFont);
    TTF_Quit();

    SDL_RenderPresent(engine->renderer());
}

void Menu::handleEvent(Engine* engine, SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_DOWN)
        {
            m_selected_option = (m_selected_option + 1) % m_options.size();
        }
        else if (event.key.keysym.sym == SDLK_UP)
        {
            m_selected_option = (m_selected_option - 1 + m_options.size()) % m_options.size();
        }
        else if (event.key.keysym.sym == SDLK_RETURN)
        {
            if (m_options[m_selected_option] == "Exit")
            {
                std::cout << "Exiting game..." << std::endl;
                engine->exit();
            }
            else if (m_options[m_selected_option] == "Start Game")
            {
                std::cout << "Starting game..." << std::endl;

                // Construct a level
                std::vector<Tile> tiles;
                for (int y = 0; y < 20; y++)
                {
                    for (int x = 0; x < 20; x++)
                    {
                        tiles.push_back(Tile(TileId::PLAIN, x, y));
                    }
                }

                // Fill the edges with water
                for (size_t n = 0; n < 20; n++)
                {
                    // Vertical
                    tiles.at(n * 20) = Tile(TileId::WATER, 0, n);
                    tiles.at(n * 20 + 19) = Tile(TileId::WATER, 19, n);
                    // Horizontal
                    tiles.at(n) = Tile(TileId::WATER, n, 0);
                    tiles.at(19 * 20 + n) = Tile(TileId::WATER, n, 19);
                }

                // Make the edges cliffs
                for (size_t n = 1; n < 19; n++)
                {
                    // Vertical
                    tiles.at(n * 20 + 1) = Tile(TileId::CLIFF_RIGHT, 1, n);
                    tiles.at(n * 20 + 18) = Tile(TileId::CLIFF_LEFT, 18, n);

                    // Horizontal
                    tiles.at(20 + n) = Tile(TileId::CLIFF_BOTTOM, n, 1);
                    tiles.at(18 * 20 + n) = Tile(TileId::CLIFF_TOP, n, 18);
                }

                // Fix the corners
                tiles.at(20 + 1) = Tile(TileId::CLIFF_CORNER_TOP_LEFT, 1, 1);
                tiles.at(20 + 18) = Tile(TileId::CLIFF_CORNER_TOP_RIGHT, 18, 1);
                tiles.at(18 * 20 + 1) = Tile(TileId::CLIFF_CORNER_BOTTOM_LEFT, 1, 18);
                tiles.at(18 * 20 + 18) = Tile(TileId::CLIFF_CORNER_BOTTOM_RIGHT, 18, 18);

                // Buildings
                std::vector<Building> buildings;

                for (int y = 0; y < 6; y++)
                {
                    for (int x = 0; x < 5; x++)
                    {
                        BuildingId      id = static_cast<BuildingId>(x);
                        BuildingFaction faction = static_cast<BuildingFaction>(y);

                        buildings.push_back(Building(3 + x, 3 + 2 * y, id, faction));
                    }
                }

                // Units
                std::vector<Unit> units;

                for (int y = 0; y < 19; y++)
                {
                    for (int x = 0; x < 6; x++)
                    {
                        units.push_back(Unit(
                            x + 9, y + 2, UnitFaction::URED, static_cast<UnitId>(y),
                            static_cast<UnitState>(x)));
                    }
                }

                std::vector<Effect> effects(
                    {Effect(3, 15, EffectId::LAND_EXPLOSION, false),
                     Effect(5, 15, EffectId::AIR_EXPLOSION, true),
                     Effect(5, 18, EffectId::NAVAL_EXPLOSION, true)});

                std::shared_ptr<Level> level =
                    std::make_shared<Level>("Osnabrück", 20, 20, tiles, buildings, units, effects);

                engine->pushScene(level);
            }
            else if (m_options[m_selected_option] == "Options")
            {
                std::cout << "Opening options..." << std::endl;
            }
        }
    }
}

void Menu::loadBackground(SDL_Renderer* renderer, const std::string& imagePath)
{
    // Lade das Hintergrundbild
    SDL_Surface* backgroundSurface = IMG_Load(imagePath.c_str());
    if (!backgroundSurface)
    {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
        return;
    }

    // Erstelle eine Textur aus der Oberfläche und speichere sie als
    // Klassenmitglied
    m_background_texture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface); // Oberfläche freigeben, da sie nicht mehr
                                        // benötigt wird

    if (!m_background_texture)
    {
        std::cerr << "Failed to create background texture: " << SDL_GetError() << std::endl;
    }
}

} // namespace advanced_wars
