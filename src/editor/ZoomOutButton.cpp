#include "ZoomOutButton.hpp"
#include "EventBroker.hpp"

ZoomOutButton::ZoomOutButton(QWidget *parent) : QPushButton("-", parent)
{
    setFixedWidth(20);

}

void ZoomOutButton::mousePressEvent(QMouseEvent *event)
{
    EventBroker::send([](EventBroker* e){e->onZoomed(-0.25);});
}