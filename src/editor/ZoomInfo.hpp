/**
* ZoomInfo.cpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QPushButton>

#include "EventHandler.hpp"

namespace editor
{

class ZoomInfo : public QPushButton, public EventHandler {
public:
    ZoomInfo(QWidget* parent = nullptr);
private:
    double current_scale;
    void onZoomed(double delta) override;
};

} // namespace editor