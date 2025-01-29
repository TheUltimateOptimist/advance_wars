#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include "SpriteProvider.hpp"
#include <QGridLayout>

class TileSelector : public QScrollArea {
public:
    TileSelector(QWidget *parent = nullptr);
    
private:
    QLabel *createNewLabel(QWidget* parent, const char *text);
    QPushButton *createButton(QWidget* parent, uint8_t id);

    template<typename T>
    void sectionLayout(QGridLayout*& layout, int usedIdCounter, QWidget* parent, T id);
    template<typename T, typename... Rest>
    void sectionLayout(QGridLayout*& layout, int usedIdCounter, QWidget* parent, T id, Rest...ids);
    template<typename... T>
    QGridLayout* creatSectionLayout(QWidget* parent, T... ids);
};