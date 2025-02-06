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
}

void AutomateButton::mousePressEvent(QMouseEvent *event)
{
    QCheckBox::mousePressEvent(event);
    std::cout << "clicked" << std::endl;
    EventHandler::send([](EventHandler* e) { e->onCheckBoxToggled(); });
}

} // namespace editor