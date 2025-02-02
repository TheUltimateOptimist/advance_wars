#include <QPushButton>

class ZoomOutButton : public QPushButton {
public:
    ZoomOutButton(QWidget* parent = nullptr);
private:
    void mousePressEvent(QMouseEvent* event) override;
};