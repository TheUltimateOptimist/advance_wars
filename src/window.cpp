#include "window.hpp"
#include <stdexcept>

namespace advanced_wars
{

Window::Window(std::string title, int w, int h)
{
    /// Init width and height
    width = w;
    height = h;

    // Generate SDL main window
    window = SDL_CreateWindow(
                title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                width,
                height,
                SDL_WINDOW_SHOWN );

    if(window == nullptr)
    {
        throw std::runtime_error("SDL window could not be generated: " + std::string(SDL_GetError()));
    }
}


int Window::w()
{
    return width;
}

int Window::h()
{
    return height;
}

SDL_Window* Window::sdl_window()
{
    return window;
}

Window::~Window()
{
    if(window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

}
