#include <QPushButton>
#include "EventBroker.hpp"

class SaveButton : public QPushButton, public EventBroker {
public:
    SaveButton(const std::string& title, QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent* event) override;
};