/**
* EventBroker.cpp
*
* @date 29.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QPushButton>

namespace editor
{

class SaveButton : public QPushButton {
public:
    SaveButton(const std::string& title, QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent* event) override;
};

} // namespace editor