/**
* AdvancedPlacementSwitch.hpp
*
* @date 06.02.2025
* @author Nils Jonathan Friedrich Eckardt implementation
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de) small changes, doc comments
*/

#pragma once

#include <QCheckBox>
#include <QString>
#include <QEvent>

namespace editor
{

/**
 * The AdvancedPlacementSwitch is displayed in the TopBar and allows the user to switch between
 * regular tile placement and advanced tile placement.
 */
class AdvancedPlacementSwitch : public QCheckBox {
public:
    /**
     * Creates an AdvancedPlacementSwitch as QCheckBox using the provided text and parent.
     * @param text The description to display next to the CheckBox.
     * @param parent The parent for this widget.
     */
    AdvancedPlacementSwitch(const QString text, QWidget *parent = nullptr);
protected:
    /**
     * Receives a QMouseEvent when the QCheckBox is pressed and subsequently emits
     * the onAdvancedPlacementToggled event method.
     * @param event The QMouseEvent for the CheckBox Pressing.
     */
    void mousePressEvent(QMouseEvent *event) override;
};

} // namespace editor