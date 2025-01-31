#include "engine.hpp"
#include "SDL_events.h"
#include "SDL_timer.h"
#include "scene.hpp"
#include "spritesheet.hpp"
#include "window.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <deque>
#include <memory>
#include <optional>
#include <stdexcept>

namespace advanced_wars
{

Engine::Engine(Window& window) : window(window), quit(false)
{

    this->sdl_renderer = SDL_CreateRenderer(
        this->window.sdl_window(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (sdl_renderer == nullptr)
    {
        throw std::runtime_error("SDL could not generate renderer: " + std::string(SDL_GetError()));
    }
}

std::deque<SDL_Event>& Engine::events()
{
    return this->_events;
}

void Engine::push_scene(std::shared_ptr<Scene> scene)
{
    this->scenes.push_back(scene);
}

void Engine::return_to_menu()
{
    // TODO: discuss if we outsource this to a separate function
    // clear everything except the first scene
    while (this->scenes.size() > 1)
    {
        this->scenes.pop_back();
    }
}

std::optional<std::shared_ptr<Scene>> Engine::pop_scene()
{
    if (this->scenes.empty())
    {
        return std::nullopt;
    }
    std::shared_ptr<Scene> tmp = scenes.back();
    this->scenes.pop_back();

    return tmp;
}

void Engine::set_spritesheet(Spritesheet& spritesheet)
{
    this->spritesheet = &spritesheet;
}

void Engine::pump()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            this->quit = true;
        }
        else
        {
            this->_events.push_back(e);
        }
    }
}

void Engine::exit()
{
    this->quit = true;
}

bool Engine::exited()
{
    return this->quit;
}

void Engine::render()
{
    if (SDL_RenderClear(this->sdl_renderer) != 0)
    {
        throw std::runtime_error("Could not clear renderer: " + std::string(SDL_GetError()));
    }

    std::shared_ptr<Scene> currentScene = scenes.back();

    currentScene->render(this);

    SDL_RenderPresent(this->sdl_renderer);
}

int Engine::get_stage()
{
    return SDL_GetTicks() / 300;
}

Spritesheet* Engine::get_spritesheet()
{
    return spritesheet.value();
}

SDL_Renderer* Engine::renderer()
{
    return this->sdl_renderer;
}

Engine::~Engine()
{
    SDL_DestroyRenderer(sdl_renderer);
    IMG_Quit();
    SDL_Quit();
}

} // namespace advanced_wars
