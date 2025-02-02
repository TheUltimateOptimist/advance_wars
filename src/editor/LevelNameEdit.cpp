#include "LevelNameEdit.hpp"
#include <QKeyEvent>
#include "EventBroker.hpp"

namespace editor 
{

LevelNameEdit::LevelNameEdit(const std::string &level_name, QWidget *parent) : QLineEdit(parent){
    this->setFixedWidth(150);
    this->setText(level_name.c_str());
}

void LevelNameEdit::keyPressEvent(QKeyEvent *event){
    QLineEdit::keyPressEvent(event);
    std::string new_name = this->text().toStdString();
    EventBroker::send([new_name](EventBroker* e){e->onLevelNameUpdated(new_name);});
}

} // namespace editor