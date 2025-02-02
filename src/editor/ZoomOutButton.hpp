#pragma once

#include <QPushButton>

namespace editor
{

class ZoomOutButton : public QPushButton {
public:
    ZoomOutButton(QWidget* parent = nullptr);
private:
    void mousePressEvent(QMouseEvent* event) override;
};

} // namespace editor