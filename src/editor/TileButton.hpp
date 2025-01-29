#include <QPushButton>
#include "EventBroker.hpp"

class TileButton : public QPushButton, public EventBroker {
public:
    TileButton(const uint8_t id, QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent* event) override;
private:
    uint8_t id;
};