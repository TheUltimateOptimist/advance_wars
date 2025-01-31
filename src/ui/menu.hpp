#pragma once

#include "../scene.hpp"
#include <SDL.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>

namespace advanced_wars
{

/**
 * @class Menu
 * @brief Represents the main menu of the game, allowing navigation between different options.
 *
 * This menu provides three selectable options:
 * - "Start Game": Begins a new game session.
 * - "Options": Opens the game settings.
 * - "Exit": Closes the application.
 */
class Menu : public Scene
{
    private:
        size_t m_selectedOption;              ///< Index of the currently selected menu option.
        std::array<std::string, 3> m_options; ///< The available menu options.
        SDL_Texture* m_backgroundTexture;    ///< Pointer to the background texture (if any).

    public:
        /**
         * @brief Constructs the Menu with an initial selected option.
         *
         * Initializes the menu with the available options and sets the currently
         * selected option based on the given index.
         *
         * @param selectedOption The index of the initially selected menu option.
         */
        Menu(int selectedOption);

        /**
         * @brief Renders the menu on the screen.
         *
         * This method clears the screen, draws the background (if available),
         * renders the menu title, and displays the selectable options. The currently
         * selected option is highlighted in a different color.
         *
         * @param engine Pointer to the game engine, used for rendering.
         */
        void render(Engine* engine) override;

        /**
         * @brief Handles user input events for menu navigation.
         *
         * This method processes keyboard input to navigate through the menu options.
         * - **Arrow Down (`SDLK_DOWN`)**: Moves the selection to the next option.
         * - **Arrow Up (`SDLK_UP`)**: Moves the selection to the previous option.
         * - **Enter (`SDLK_RETURN`)**: Confirms the selection:
         *   - **"Start Game"**: Loads the game scene.
         *   - **"Options"**: Opens the settings menu.
         *   - **"Exit"**: Closes the application.
         *
         * @param engine Pointer to the game engine, used to manage scenes.
         * @param event The SDL event containing user input data.
         */
        void handleEvent(Engine* engine, SDL_Event& event);

        /**
         * @brief Loads a background image as a texture.
         *
         * This method loads an image file, converts it into an SDL texture, and
         * assigns it as the menu's background. If the loading fails, an error is
         * logged, and the menu will display a plain black background instead.
         *
         * @param renderer The SDL renderer used to create the texture.
         * @param imagePath The file path to the background image.
         */
        void loadBackground(SDL_Renderer* renderer, const std::string& imagePath);

        /**
         * @brief Destroys the menu and releases resources.
         *
         * Cleans up allocated resources, including the background texture (if loaded),
         * and ensures that SDL_Image is properly shut down.
         */
        ~Menu();
};

} // namespace advanced_wars