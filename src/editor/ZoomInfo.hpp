#include <QPushButton>
#include "EventBroker.hpp"

class ZoomInfo : public QPushButton, public EventBroker {
public:
    ZoomInfo(QWidget* parent = nullptr);
private:
    double current_scale;
    void onZoomed(double delta) override;
};