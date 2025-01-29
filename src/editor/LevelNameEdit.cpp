#include "LevelNameEdit.hpp"
#include <QKeyEvent>

LevelNameEdit::LevelNameEdit(const std::string &level_name, QWidget *parent) : QLineEdit(parent){
    this->setFixedWidth(150);
    this->setText(level_name.c_str());
}

void LevelNameEdit::keyPressEvent(QKeyEvent *event){
    QLineEdit::keyPressEvent(event);
    sendLevelNameUpdated(this->text().toStdString());
}
