#pragma once

#include <QPushButton>

class ZoomInButton : public QPushButton {
public:
    ZoomInButton(QWidget* parent = nullptr);
private:
    void mousePressEvent(QMouseEvent* event) override;
};