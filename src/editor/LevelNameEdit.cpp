/**
* LevelNameEdit.cpp
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
    setFixedWidth(150);
    setText(level_name.c_str()); // prefill level name
}

void LevelNameEdit::keyPressEvent(QKeyEvent* event)
{
    QLineEdit::keyPressEvent(event);
    std::string new_name = text().toStdString();
    // dispatch the level name updated event method to allow all depending Widgets to take action
    EventHandler::send([new_name](EventHandler* e) { e->onLevelNameUpdated(new_name); });
}

} // namespace editor