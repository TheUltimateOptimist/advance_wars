#pragma once

#include <QLabel>

#include "EventHandler.hpp"

namespace editor
{

class LevelTitle : public QLabel, EventHandler {
public:
    LevelTitle(int width, int height, const std::string& name, QWidget *parent = nullptr);
    void onLevelNameUpdated(std::string name) override;   
    void setTitle(const std::string& name);
private:
    int m_width;
    int m_height;
};

} // namespace editor