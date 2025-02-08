/**
* LevelNameEdit.hpp
*
* @date 29.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QLineEdit>

namespace editor 
{

/**
 * The LevelNameEdit Widget is displayed on the left of the TopBar.
 * It allows the user to set a custom level name.
 */
class LevelNameEdit : public QLineEdit {
public:
    /**
     * Creates a LevelNameEdit Widget prefilling the provided level_name.
     * The parent of the LevelNameEdit Widget is set to the provided parent.
     * @param level_name The initial name of the level.
     * @param parent The Widget that should be set as the parent of this Widget.
     */
    LevelNameEdit(const std::string& level_name, QWidget* parent = nullptr);
protected:
    /**
     * Receives a QKeyEvent when the user presses a key to change the content of the QLineEdit.
     * The onLevelNameUpdated event method is subsequently emitted. 
     * @param event The QKeyEvent for the Key Press.
     */
    void keyPressEvent(QKeyEvent* event) override;
};

} // namespace editor