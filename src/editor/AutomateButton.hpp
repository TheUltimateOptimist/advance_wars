/**
* TileSelector.hpp
*
* @date 06.02.2025
* @author Nils Jonathan Friedrich Eckardt implementation
*/

#pragma once

#include <QCheckBox>
#include <QString>
#include <QEvent>

namespace editor
{

class AutomateButton : public QCheckBox {
public:
    AutomateButton(const QString text, QWidget *parent = nullptr);
protected:
    void changeEvent(QEvent* event) override;
private:
    bool toggled;
};

} // namespace editor