#include "TopBar.hpp"
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QSize>
#include <QPushButton>

TopBar::TopBar(const std::string& level_name, int level_width, int level_height, QWidget *parent) : QToolBar(parent) {
    QWidget *container = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(this);
    QLineEdit *text_field = new QLineEdit(this);
    text_field->setFixedWidth(150);
    text_field->setText(level_name.c_str());
    QLabel *field_label = new QLabel("Name: ", this);
    QLabel *dimensions = new QLabel((std::to_string(level_width) + " X " + std::to_string(level_height)).c_str(), this);
    QPushButton *save_button = new QPushButton("Speichern", this);
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

