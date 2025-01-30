#include "TopBar.hpp"
#include <QLabel>
#include <QHBoxLayout>
#include <QSize>
#include "SaveButton.hpp"
#include "LevelNameEdit.hpp"

TopBar::TopBar(const std::string& level_name, int level_width, int level_height, QWidget *parent) : QToolBar(parent) {
    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(container);

    QLabel *field_label = new QLabel("Name: ", this);
    LevelNameEdit *text_field = new LevelNameEdit(level_name, this);
    QLabel *dimensions = new QLabel((std::to_string(level_width) + " X " + std::to_string(level_height)).c_str(), this);
    QPushButton *save_button = new SaveButton("Speichern", this);

    layout->addWidget(field_label);
    layout->addWidget(text_field);
    layout->addSpacing(15);
    layout->addWidget(dimensions);
    layout->addSpacing(15);
    layout->addWidget(save_button);
    layout->addStretch();
    
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);  // Allow resizing
    container->setLayout(layout);
    this->addWidget(container);
}

