#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <SDL.h>

namespace advanced_wars
{

/**
 * @brief The main window of the game
 */
class Window
{
public:

    /***
     * Creates a main window with given \ref title, width \ref w and height \ref h
     *
     * @param title		Title of the window
     * @param w			Width
     * @param h			Height
     */
    Window(std::string title, int w, int h);

    /**
     * Forbids the creation of copies of a window
     */
    Window(const Window&) = delete;
    //Window& operator=(const Window&) = delete;

    /***
     * Destructor.
     */
    ~Window();

    /// Retruns the current width of the window
    int w();

    /// Returns the current height of the window
    int h();

    SDL_Window* sdl_window();

private:

    /// SDL main window struct
    SDL_Window*	window;

    /// Window width
    int width;

    /// Window height
    int	height;
};

}

#endif
