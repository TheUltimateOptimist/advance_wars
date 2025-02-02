#pragma once

#include <QPushButton>

namespace editor
{

class TileButton : public QPushButton {
public:
    TileButton(const uint8_t id, QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent* event) override;
private:
    uint8_t id;
};

} // namespace editor