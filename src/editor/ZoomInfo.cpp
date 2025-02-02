#include "ZoomInfo.hpp"

ZoomInfo::ZoomInfo(QWidget *parent) : QPushButton(parent), current_scale(2)
{
    setDisabled(true);
    setText((std::to_string((int) std::round(current_scale*100)) + "%").c_str());
    setFixedWidth(50);
}

void ZoomInfo::onZoomed(double delta)
{
    if (current_scale + delta <= 0) return;
    current_scale += delta;
    setText((std::to_string((int) std::round(current_scale*100)) + " %").c_str());
}
