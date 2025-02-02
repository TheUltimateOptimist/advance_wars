#pragma once

#include <QToolBar>

class TopBar : public QToolBar {
public: 
    TopBar(const std::string& level_name, QWidget *parent = nullptr);
};