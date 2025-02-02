#pragma once

#include <QPushButton>

class SaveButton : public QPushButton {
public:
    SaveButton(const std::string& title, QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent* event) override;
};