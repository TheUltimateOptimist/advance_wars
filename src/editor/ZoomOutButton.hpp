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

/**
 * The ZoomOutButton is a part of the TopBar.
 * It listens to mouse presses and emits the onZoomed event method
 * with a delta of -0.25 in case of a mouse press.
 */
class ZoomOutButton : public QPushButton {
public:
    /**
    * Creates a ZoomInButton with the provided parent.
    * @param parent The parent to set for the ZoomInButton.
    */
    ZoomOutButton(QWidget* parent = nullptr);

private:
    /**
     * When a mouse press event is received,
     * it emits the onZoomed event method with a delta of 0.25.
     */
    void mousePressEvent(QMouseEvent*) override;
};

} // namespace editor