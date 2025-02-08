/**
* SaveButton.cpp
*
* @date 29.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QPushButton>

namespace editor
{

/**
 * The Save Button is displayed on the top right in the TopBar. 
 * Clicking it opens the file save dialog. 
 * It is not responsible for writing the level itself. 
 * Instead it collects the desired file path from the file save dialog and
 * emits the onLevelWriteRequested event method which is overwritten by the LevelScene.
 */
class SaveButton : public QPushButton {
public:
    /**
     * Creates a SaveButton with the given title and parent.
     * @param title The title to display inside the SaveButton.
     * @param parent The parent that should be set for the SaveButton.
     */
    SaveButton(const std::string& title, QWidget *parent = nullptr);

protected:
    /**
     * Opens the file save dialog, collects the desired file path and emits
     * the onLevelWriteRequested event method.
     * @param event The QMouseEvent for the mouse press.
     */
    void mousePressEvent(QMouseEvent* event) override;
};

} // namespace editor