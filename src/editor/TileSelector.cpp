#include "TileSelector.hpp"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "SpriteProvider.hpp"

TileSelector::TileSelector(QWidget * parent) : QScrollArea(parent){
    this->setFixedWidth(300);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    QWidget *tilebox = new QWidget;

    QLabel *labelGround = createNewLabel(this, "Terrain");
    QLabel *labelStreet = createNewLabel(this, "Straßen");
    QLabel *labelWater = createNewLabel(this, "Wasser");
    QLabel *labelNeutral = createNewLabel(this, "Neutrale Gebäude");
    QLabel *labelRed = createNewLabel(this, "Rote Gebäude");
    QLabel *labelYellow = createNewLabel(this, "Gelbe Gebäude");
    QLabel *labelGreen = createNewLabel(this, "Grüne Gebäude");
    QLabel *labelBlue = createNewLabel(this, "Blaue Gebäude");
    QLabel *labelPurple = createNewLabel(this, "Violette Gebäude");

    QPushButton *button0 = createButton(this, 0);
    QPushButton *button2 = createButton(this, 2);
    //ISSUE: only the first button is created for some reason


    QVBoxLayout *layout = new QVBoxLayout(tilebox);
    layout -> addWidget(labelGround);
    layout -> addWidget(button0);
    //TileID 0,2,3
    layout -> addWidget(labelStreet);
    layout -> addWidget(button2);
    //TileID 6,7,13,14,15,16,8,9,10,11,12,4,5
    layout -> addWidget(labelWater);
    //TileID 1,17,18,19,20,21,22,26,23,27,24,28,25,29
    layout -> addWidget(labelNeutral);
    //TileID 75,76,77,78,79
    layout -> addWidget(labelRed);
    //TileID 50,51,52,53,54
    layout -> addWidget(labelYellow);
    //TileID 60,61,62,63,64
    layout -> addWidget(labelGreen);
    //TileID 65,66,67,68,69
    layout -> addWidget(labelBlue);
    //TileID 55,56,57,58,59
    layout -> addWidget(labelPurple);
    //TileID 70,71,72,73,74
    tilebox -> show();

    //QScrollArea *scrollArea = new QScrollArea;
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
    QPixmap scaledpixmap = pixmap.scaled(80,80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPushButton *button =new QPushButton(parent);
    button -> setIcon(QIcon(scaledpixmap));
    button -> setIconSize(QSize(80,80));
    button -> setFixedSize(80,80);
    return button;
}