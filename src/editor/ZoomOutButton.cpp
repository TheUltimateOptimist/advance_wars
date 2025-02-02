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