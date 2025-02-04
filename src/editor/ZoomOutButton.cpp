/**
* ZoomOutButton.cpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "ZoomOutButton.hpp"
#include "EventBroker.hpp"

namespace editor
{

ZoomOutButton::ZoomOutButton(QWidget* parent) : QPushButton("-", parent)
{
    setFixedWidth(20);
}

void ZoomOutButton::mousePressEvent(QMouseEvent* event)
{
    EventBroker::send([](EventBroker* e) { e->onZoomed(-0.25); });
}

} // namespace editor