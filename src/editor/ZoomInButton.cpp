#include "ZoomInButton.hpp"
#include "EventBroker.hpp"

ZoomInButton::ZoomInButton(QWidget *parent) : QPushButton("+", parent)
{
    setFixedWidth(20);

}

void ZoomInButton::mousePressEvent(QMouseEvent *event)
{
    EventBroker::send([](EventBroker* e){e->onZoomed(0.25);});
}
