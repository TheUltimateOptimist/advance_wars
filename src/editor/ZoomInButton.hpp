#pragma once

#include <QPushButton>

namespace editor
{

class ZoomInButton : public QPushButton {
public:
    ZoomInButton(QWidget* parent = nullptr);
private:
    void mousePressEvent(QMouseEvent* event) override;
};

} // namespace editor