#include "Window.hpp"
#include <stdexcept>

namespace advanced_wars
{

Window::Window(std::string title, int w, int h)
{
    /// Init width and height
    m_width = w;
    m_height = h;

    // Generate SDL main window
    m_window = SDL_CreateWindow(
        title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height,
        SDL_WINDOW_SHOWN);

    if (m_window == nullptr)
    {
        throw std::runtime_error(
            "SDL window could not be generated: " + std::string(SDL_GetError()));
    }
}

int Window::w()
{
    return m_width;
}

int Window::h()
{
    return m_height;
}

SDL_Window* Window::sdl_window()
{
    return m_window;
}

Window::~Window()
{
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
}

} // namespace advanced_wars
