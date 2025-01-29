#include "SaveButton.hpp"

SaveButton::SaveButton(const std::string& title, QWidget *parent) : QPushButton(title.c_str(), parent){}

void SaveButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    sendLevelWriteRequested();
}
