#include "SaveButton.hpp"
#include "EventBroker.hpp"

SaveButton::SaveButton(const std::string& title, QWidget *parent) : QPushButton(title.c_str(), parent){}

void SaveButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    EventBroker::send([](EventBroker* e){e->onLevelWriteRequested();});
}
