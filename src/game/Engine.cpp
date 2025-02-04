#include "Engine.hpp"
#include "SDL_events.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include "Scene.hpp"
#include "Spritesheet.hpp"
#include "Window.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <cmath>
#include <deque>
#include <memory>
#include <optional>
#include <stdexcept>

namespace advanced_wars
{

Engine::Engine(Window& window) : m_window(window), m_quit(false)
{

    this->m_SDLRenderer = SDL_CreateRenderer(
        this->m_window.sdl_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (m_SDLRenderer == nullptr)
    {
        throw std::runtime_error("SDL could not generate renderer: " + std::string(SDL_GetError()));
    }
}

float Engine::getDPI()
{
    float ddpi;
    if (SDL_GetDisplayDPI(
            SDL_GetWindowDisplayIndex(this->m_window.sdl_window()), &ddpi, nullptr, nullptr) != 0)
    {
        throw std::runtime_error("SDL could not get DPI: " + std::string(SDL_GetError()));
    }

    return ddpi;
}

int Engine::getRenderingScale()
{
    return (int)std::round(this->getDPI() / 32.0);
}

std::deque<SDL_Event>& Engine::events()
{
    return this->m_events;
}

void Engine::pushScene(std::shared_ptr<Scene> scene)
{
    this->m_scenes.push_back(scene);
}

void Engine::returnToMenu()
{
    // TODO: discuss if we outsource this to a separate function
    // clear everything except the first scene
    while (this->m_scenes.size() > 1)
    {
        this->m_scenes.pop_back();
    }
}

std::optional<std::shared_ptr<Scene>> Engine::popScene()
{
    if (this->m_scenes.empty())
    {
        return std::nullopt;
    }
    std::shared_ptr<Scene> tmp = m_scenes.back();
    this->m_scenes.pop_back();

    return tmp;
}

void Engine::setSpritesheet(Spritesheet& spritesheet)
{
    this->m_spritesheet = &spritesheet;
}

void Engine::pump()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            this->m_quit = true;
        }
        else
        {
            this->m_events.push_back(e);
        }
    }
    std::shared_ptr<Scene> currentScene = m_scenes.back();
    while (m_events.size() > 0)
    {
        currentScene->handleEvent(*this, m_events.at(0));
        m_events.pop_front();
    }
}

void Engine::exit()
{
    this->m_quit = true;
}

bool Engine::exited()
{
    return this->m_quit;
}

void Engine::render()
{
    if (SDL_RenderClear(this->m_SDLRenderer) != 0)
    {
        throw std::runtime_error("Could not clear renderer: " + std::string(SDL_GetError()));
    }

    std::shared_ptr<Scene> currentScene = m_scenes.back();

    currentScene->render(*this);

    SDL_RenderPresent(this->m_SDLRenderer);
}

int Engine::getStage()
{
    return SDL_GetTicks() / 300;
}

Spritesheet* Engine::getSpritesheet()
{
    return m_spritesheet.value();
}

SDL_Renderer* Engine::renderer()
{
    return this->m_SDLRenderer;
}

Engine::~Engine()
{
    SDL_DestroyRenderer(m_SDLRenderer);
    IMG_Quit();
    SDL_Quit();
}

} // namespace advanced_wars
