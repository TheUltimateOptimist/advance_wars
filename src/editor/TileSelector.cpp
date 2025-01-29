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

    QPushButton* buttons[80];
    for(uint8_t i=0;i<30;i++){
        buttons[i] = createButton(this, i);
    }
    for(uint8_t i=50;i<80;i++){
        buttons[i] = createButton(this, i);
    } 


    QVBoxLayout *layout = new QVBoxLayout(tilebox);
    //Terrain
    //0,2,3
    layout -> addWidget(labelGround);
    layout -> addWidget(buttons[0]);
    layout -> addWidget(buttons[2]);
    layout -> addWidget(buttons[3]);
    //Straßen
    //TileID 6,7,13,14,15,16,8,9,10,11,12,4,5
    layout -> addWidget(labelStreet);
    layout -> addWidget(buttons[6]);
    layout -> addWidget(buttons[7]);
    layout -> addWidget(buttons[13]);
    layout -> addWidget(buttons[14]);
    layout -> addWidget(buttons[15]);
    layout -> addWidget(buttons[16]);
    layout -> addWidget(buttons[8]);
    layout -> addWidget(buttons[9]);
    layout -> addWidget(buttons[10]);
    layout -> addWidget(buttons[11]);
    layout -> addWidget(buttons[12]);
    layout -> addWidget(buttons[4]);
    layout -> addWidget(buttons[5]);
    //Wasser
    //TileID 1,17,18,19,20,21,22,26,23,27,24,28,25,29
    layout -> addWidget(labelWater);
    layout -> addWidget(buttons[1]);
    layout -> addWidget(buttons[17]);
    layout -> addWidget(buttons[18]);
    layout -> addWidget(buttons[19]);
    layout -> addWidget(buttons[20]);
    layout -> addWidget(buttons[21]);
    layout -> addWidget(buttons[22]);
    layout -> addWidget(buttons[26]);
    layout -> addWidget(buttons[23]);
    layout -> addWidget(buttons[27]);
    layout -> addWidget(buttons[24]);
    layout -> addWidget(buttons[28]);
    layout -> addWidget(buttons[25]);
    layout -> addWidget(buttons[29]);
    //Neutral
    //TileID 75,76,77,78,79
    layout -> addWidget(labelNeutral);
    layout -> addWidget(buttons[75]);
    layout -> addWidget(buttons[76]);
    layout -> addWidget(buttons[77]);
    layout -> addWidget(buttons[78]);
    layout -> addWidget(buttons[79]);
    //Rot
    //TileID 50,51,52,53,54
    layout -> addWidget(labelRed);
    layout -> addWidget(buttons[50]);
    layout -> addWidget(buttons[51]);
    layout -> addWidget(buttons[52]);
    layout -> addWidget(buttons[53]);
    layout -> addWidget(buttons[54]);
    //Gelb
    //TileID 60,61,62,63,64
    layout -> addWidget(labelYellow);
    layout -> addWidget(buttons[60]);
    layout -> addWidget(buttons[61]);
    layout -> addWidget(buttons[62]);
    layout -> addWidget(buttons[63]);
    layout -> addWidget(buttons[64]);
    //Grün
    //TileID 65,66,67,68,69
    layout -> addWidget(labelGreen);
    layout -> addWidget(buttons[65]);
    layout -> addWidget(buttons[66]);
    layout -> addWidget(buttons[67]);
    layout -> addWidget(buttons[68]);
    layout -> addWidget(buttons[69]);
    //Blau
    //TileID 55,56,57,58,59
    layout -> addWidget(labelBlue);
    layout -> addWidget(buttons[55]);
    layout -> addWidget(buttons[56]);
    layout -> addWidget(buttons[57]);
    layout -> addWidget(buttons[58]);
    layout -> addWidget(buttons[59]);
    //Violett
    //TileID 70,71,72,73,74
    layout -> addWidget(labelPurple);
    layout -> addWidget(buttons[70]);
    layout -> addWidget(buttons[71]);
    layout -> addWidget(buttons[72]);
    layout -> addWidget(buttons[73]);
    layout -> addWidget(buttons[74]);
    
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


    

    