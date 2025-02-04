/**
* TileSelector.hpp
*
* @date 29.01.2025
* @author Nils Jonathan Friedrich Eckardt implementation
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de) minor changes
*/

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
    uint8_t m_id;
};

} // namespace editor