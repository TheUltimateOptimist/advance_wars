/**
* TopBar.cpp
*
* @date 28.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "TopBar.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QSize>

#include "LevelNameEdit.hpp"
#include "LevelTitle.hpp"
#include "SaveButton.hpp"
#include "ZoomInButton.hpp"
#include "ZoomInfo.hpp"
#include "ZoomOutButton.hpp"

namespace editor
{

TopBar::TopBar(const std::string& level_name, QWidget* parent) : QToolBar(parent)
{
    QWidget*     container = new QWidget(this);
    QHBoxLayout* main_layout = new QHBoxLayout(container);
    QWidget*     left_container = new QWidget(container);
    QWidget*     right_container = new QWidget(container);
    QHBoxLayout* left_layout = new QHBoxLayout(left_container);
    QHBoxLayout* right_layout = new QHBoxLayout(right_container);

    QLabel*        field_label = new QLabel("Name: ", left_container);
    LevelNameEdit* text_field = new LevelNameEdit(level_name, left_container);
    left_layout->addWidget(field_label);
    left_layout->addWidget(text_field);
    left_layout->addStretch();

    QPushButton* save_button = new SaveButton("Speichern", right_container);
    QPushButton* zoom_in = new ZoomInButton(right_container);
    QPushButton* zoom_out = new ZoomOutButton(right_container);
    QPushButton* zoom_info = new ZoomInfo(right_container);
    right_layout->addStretch();
    right_layout->addWidget(zoom_out);
    right_layout->addWidget(zoom_info);
    right_layout->addWidget(zoom_in);
    right_layout->addStretch();
    right_layout->addWidget(save_button);
    right_layout->addStretch();

    right_container->setFixedWidth(300);
    main_layout->addWidget(left_container, 1);
    main_layout->addWidget(right_container);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); // Allow resizing
    this->addWidget(container);
}

} // namespace editor
