#include <QPushButton>
#include "EventBroker.hpp"

class TileButton : public QPushButton, public EventBroker {
public:
    TileButton(QWidget *parent = nullptr, const uint8_t id);
protected:
    void mousePressEvent(QMouseEvent* event) override;
};