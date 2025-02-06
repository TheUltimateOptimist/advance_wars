/**
* TileSelector.cpp
*
* @date 06.02.2025
* @author Nils Jonathan Friedrich Eckardt implementation
*/

#include "AutomateButton.hpp"
#include "EventHandler.hpp"
#include <QMessageBox>
#include <QEvent>

namespace editor
{

AutomateButton::AutomateButton(const QString text, QWidget* parent) : QCheckBox(text,parent)
{
    setChecked(false);
    toggled = false;
}

void AutomateButton::changeEvent(QEvent* event)
{
    QCheckBox::changeEvent(event);
    toggled =!toggled;
    bool isToggled= toggled;
    EventHandler::send([isToggled](EventHandler* e) { e->onCheckBoxToggle(isToggled); });
}

} // namespace editor