#pragma once

#include <QToolBar>

namespace editor
{

class TopBar : public QToolBar {
public: 
    TopBar(const std::string& level_name, QWidget *parent = nullptr);
};

} // namespace editor