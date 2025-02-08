/**
* AdvancedPlacementSwitch.hpp
*
* @date 06.02.2025
* @author Nils Jonathan Friedrich Eckardt implementation
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de) small changes
*/

#pragma once

#include <QCheckBox>
#include <QString>
#include <QEvent>

namespace editor
{

class AdvancedPlacementSwitch : public QCheckBox {
public:
    AdvancedPlacementSwitch(const QString text, QWidget *parent = nullptr);
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

} // namespace editor