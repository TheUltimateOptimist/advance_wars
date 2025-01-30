#include "TopBar.hpp"
#include <QLabel>
#include <QHBoxLayout>
#include <QSize>
#include "SaveButton.hpp"
#include "LevelTitle.hpp"
#include "LevelNameEdit.hpp"

TopBar::TopBar(const std::string& level_name, int level_width, int level_height, QWidget *parent) : QToolBar(parent) {
    QWidget *container = new QWidget(this);
    QHBoxLayout *main_layout = new QHBoxLayout(container);

    QHBoxLayout* left_layout = new QHBoxLayout(container);
    QHBoxLayout* middle_layout = new QHBoxLayout(container);
    QHBoxLayout* right_layout = new QHBoxLayout(container);

    QLabel *field_label = new QLabel("Name: ", left_layout);
    LevelNameEdit *text_field = new LevelNameEdit(level_name, left_layout);
    left_layout->addWidget(field_label);
    left_layout->addWidget(text_field);
    left_layout->addStretch();
    LevelTitle* title = new LevelTitle(level_width, level_height, level_name, middle_layout);
    middle_layout->addStretch();
    middle_layout->addWidget(title);
    middle_layout->addStretch();
    QPushButton *save_button = new SaveButton("Speichern", right_layout);
    right_layout->addStretch();
    right_layout->addWidget(save_button); 

    main_layout->addLayout(left_layout, 1);
    main_layout->addLayout(middle_layout, 1);
    main_layout->addLayout(right_layout, 1);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);  // Allow resizing
    this->addWidget(container);
}

