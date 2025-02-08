/**
* AdvancedPlacementSwitch.cpp
*
* @date 06.02.2025
* @author Nils Jonathan Friedrich Eckardt implementation
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de) small changes
*/

#include "AdvancedPlacementSwitch.hpp"
#include "EventHandler.hpp"
#include <QMessageBox>
#include <QEvent>

namespace editor
{

AdvancedPlacementSwitch::AdvancedPlacementSwitch(const QString text, QWidget* parent) : QCheckBox(text,parent)
{
    setChecked(false);
}

void AdvancedPlacementSwitch::mousePressEvent(QMouseEvent *event)
{
    QCheckBox::mousePressEvent(event);
    EventHandler::send([](EventHandler* e) { e->onAdvancedPlacementToggled(); });
}

} // namespace editor