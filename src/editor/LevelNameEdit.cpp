/**
* EventHandler.cpp
*
* @date 29.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "LevelNameEdit.hpp"

#include <QKeyEvent>

#include "EventHandler.hpp"

namespace editor
{

LevelNameEdit::LevelNameEdit(const std::string& level_name, QWidget* parent) : QLineEdit(parent)
{
    this->setFixedWidth(150);
    this->setText(level_name.c_str());
}

void LevelNameEdit::keyPressEvent(QKeyEvent* event)
{
    QLineEdit::keyPressEvent(event);
    std::string new_name = this->text().toStdString();
    EventHandler::send([new_name](EventHandler* e) { e->onLevelNameUpdated(new_name); });
}

} // namespace editor