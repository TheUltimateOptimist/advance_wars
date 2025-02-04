/**
* ZoomInButton.cpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "ZoomInButton.hpp"
#include "EventBroker.hpp"

namespace editor
{

ZoomInButton::ZoomInButton(QWidget* parent) : QPushButton("+", parent)
{
    setFixedWidth(20);
}

void ZoomInButton::mousePressEvent(QMouseEvent* event)
{
    EventBroker::send([](EventBroker* e) { e->onZoomed(0.25); });
}

} // namespace editor