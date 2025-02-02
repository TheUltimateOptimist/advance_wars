#include "LevelTitle.hpp"

namespace editor
{

LevelTitle::LevelTitle(int width, int height, const std::string &name, QWidget *parent) : QLabel(parent), m_width(width), m_height(height) {
    this->setTitle(name);
}

void LevelTitle::onLevelNameUpdated(std::string name)
{
    this->setTitle(name);
}

void LevelTitle::setTitle(const std::string &name)
{
    std::string dim_text = "(" + std::to_string(m_width) + " X " + std::to_string(m_height) + ")";
    this->setText((name + " " + dim_text).c_str());
}

} // namespace editor 