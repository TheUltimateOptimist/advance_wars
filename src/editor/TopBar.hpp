/**
* TopBar.hpp
*
* @date 28.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <QToolBar>

namespace editor
{

class TopBar : public QToolBar {
public: 
    TopBar(const std::string& level_name, QWidget *parent = nullptr);
};

} // namespace editor