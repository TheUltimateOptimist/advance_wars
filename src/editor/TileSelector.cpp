#include "TileSelector.hpp"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include "SpriteProvider.hpp"

TileSelector::TileSelector(QWidget * parent) : QScrollArea(parent){
    this->setFixedWidth(300);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget *tilebox = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(tilebox);

    layout -> addWidget(createNewLabel(this, "Terrain"));
    layout -> addLayout(creatSectionLayout(tilebox, 0,2,3));
    layout -> addWidget(createNewLabel(this, "Straßen"));
    layout -> addLayout(creatSectionLayout(tilebox, 6,7,13,14,15,16,8,9,10,11,12,4,5));
    layout -> addWidget(createNewLabel(this, "Wasser"));
    layout -> addLayout(creatSectionLayout(tilebox, 1,17,21,19,20,18,26,25,27,24,29,22,28,23));
    layout -> addWidget(createNewLabel(this, "Neutrale Gebäude"));
    layout -> addLayout(creatSectionLayout(tilebox, 75,76,77,78,79));
    layout -> addWidget(createNewLabel(this, "Rote Gebäude"));
    layout -> addLayout(creatSectionLayout(tilebox, 50,51,52,53,54));
    layout -> addWidget(createNewLabel(this, "Gelbe Gebäude"));
    layout -> addLayout(creatSectionLayout(tilebox, 60,61,62,63,64));
    layout -> addWidget(createNewLabel(this, "Grüne Gebäude"));
    layout -> addLayout(creatSectionLayout(tilebox, 65,66,67,68,69));
    layout -> addWidget(createNewLabel(this, "Blaue Gebäude"));
    layout -> addLayout(creatSectionLayout(tilebox, 55,56,57,58,59));
    layout -> addWidget(createNewLabel(this, "Violette Gebäude"));
    layout -> addLayout(creatSectionLayout(tilebox, 70,71,72,73,74));  

    tilebox -> setLayout(layout);
    tilebox -> show();

    this -> setWidget(tilebox);
}

QLabel *TileSelector::createNewLabel(QWidget* parent, const char *text){
    QLabel *label = new QLabel(parent);
    label -> setText(text);
    label -> setAlignment(Qt::AlignLeft);
    label -> setFrameStyle(QFrame::NoFrame);
    return label;
}

QPushButton *TileSelector::createButton(QWidget* parent, uint8_t id){
    QPixmap pixmap = SpriteProvider::get_sprite(id);
    QPixmap scaledpixmap = pixmap.scaled(72,72, Qt::KeepAspectRatio, Qt::FastTransformation);
    QPushButton *button =new QPushButton(parent);
    button -> setIcon(QIcon(scaledpixmap));
    button -> setIconSize(QSize(72,72));
    button -> setFixedSize(80,80);
    return button;
}

template<typename T>
void TileSelector::sectionLayout(QGridLayout*& layout, int usedIdCounter, QWidget* parent, T id){
    QPushButton* button = createButton(parent, id);
    layout -> addWidget(button,usedIdCounter/3, usedIdCounter%3);
}
    
template<typename T, typename... Rest>
void TileSelector::sectionLayout(QGridLayout*& layout, int usedIdCounter, QWidget* parent, T id, Rest...ids){
    QPushButton* button = createButton(parent, id);
    layout -> addWidget(button,usedIdCounter/3, usedIdCounter%3);
    usedIdCounter++;
    sectionLayout(layout,usedIdCounter,parent,ids...);
}

template<typename... T>
QGridLayout* TileSelector::creatSectionLayout(QWidget* parent, T... ids){
    QGridLayout *layout = new QGridLayout(parent);
    int usedIdCounter = 0;
    sectionLayout(layout,usedIdCounter,parent,ids...);
    return layout;
}
    