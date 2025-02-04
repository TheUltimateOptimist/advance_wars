/**
* ZoomOutButton.cpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "ZoomOutButton.hpp"

#include "EventHandler.hpp"

namespace editor
{

ZoomOutButton::ZoomOutButton(QWidget* parent) : QPushButton("-", parent)
{
    setFixedWidth(20);
}

void ZoomOutButton::mousePressEvent(QMouseEvent* event)
{
    EventHandler::send([](EventHandler* e) { e->onZoomed(-0.25); });
}

} // namespace editor