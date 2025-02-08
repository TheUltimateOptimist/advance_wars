/**
* ZoomOutButton.hpp
*
* @date 02.02.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

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