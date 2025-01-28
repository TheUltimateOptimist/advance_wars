#include <QApplication>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

class Tile : public QGraphicsPixmapItem {
public:
    explicit Tile(const QPixmap& pixmap, uint8_t id);

protected:
    // Override mousePressEvent to handle mouse clicks
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    
};