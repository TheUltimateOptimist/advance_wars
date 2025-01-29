#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include "../scene.hpp"
#include <SDL.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace advanced_wars {

/**
 * @class PauseMenu
 * @brief A scene that represents the in-game pause menu.
 * 
 * The pause menu allows the player to:
 * - **Resume**: Return to the current game scene.
 * - **Options**: (Currently not implemented).
 * - **Exit**: Return to the main menu.
 * 
 * The menu supports keyboard navigation and responds to user input.
 */
class PauseMenu : public Scene {
private:
  size_t selectedOption; ///< Index of the currently selected menu option.
  std::array<std::string, 3> options; ///< The available pause menu options.
  SDL_Texture *backgroundTexture; ///< Pointer to the background texture (if available).

public:
  /**
   * @brief Constructs the pause menu with a background texture.
   * 
   * The pause menu initializes the menu options and stores the provided
   * background texture. If no texture is provided, a default black background is used.
   * 
   * @param selectedOption The index of the initially selected menu option.
   * @param backgroundTexture A pointer to the background texture (can be nullptr).
   */
  PauseMenu(int selectedOption, SDL_Texture *backgroundTexture);

  /**
   * @brief Renders the pause menu on the screen.
   * 
   * This method:
   * - Draws the background (if available).
   * - Displays the menu options with the currently selected option highlighted.
   * - Presents the rendered frame to the screen.
   * 
   * @param engine Pointer to the game engine, used for rendering.
   */
  void render(Engine *engine) override;

  /**
   * @brief Handles user input events for menu navigation.
   * 
   * This method processes keyboard input to navigate and interact with the pause menu.
   * - **Arrow Down (`SDLK_DOWN`)**: Moves the selection to the next option.
   * - **Arrow Up (`SDLK_UP`)**: Moves the selection to the previous option.
   * - **Escape (`SDLK_ESCAPE`)**: Resumes the game by removing the pause menu.
   * - **Enter (`SDLK_RETURN`)**: Executes the selected option:
   *   - **"Resume"**: Closes the pause menu and resumes the game.
   *   - **"Exit"**: Returns to the main menu.
   * 
   * @param engine Pointer to the game engine, used to manage scenes.
   * @param event The SDL event containing user input data.
   */
  void handleEvent(Engine *engine, SDL_Event &event);

  /**
   * @brief Loads a background image as a texture.
   * 
   * This method loads an image file, converts it into an SDL texture, and assigns it
   * as the menu’s background. If the loading fails, an error is logged, and the menu
   * will display a plain black background instead.
   * 
   * @param renderer The SDL renderer used to create the texture.
   * @param imagePath The file path to the background image.
   */
  void loadBackground(SDL_Renderer *renderer, const std::string &imagePath);

  /**
   * @brief Destroys the pause menu and releases resources.
   * 
   * Cleans up allocated resources, including the background texture (if loaded),
   * and ensures that SDL_ttf is properly shut down.
   */
  ~PauseMenu();
};

} // namespace advanced_wars

#endif // PAUSE_MENU_HPP