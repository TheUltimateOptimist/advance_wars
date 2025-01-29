#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include "SpriteProvider.hpp"

class TileSelector : public QScrollArea {
public:
    TileSelector(QWidget *parent = nullptr);
    
private:
    QLabel *createNewLabel(QWidget* parent, const char *text);
    QPushButton *createButton(QWidget* parent, SpriteProvider &tileprovider, uint8_t id);
};