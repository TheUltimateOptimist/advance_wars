#include "LevelTitle.hpp"

LevelTitle::LevelTitle(int width, int height, const std::string &name, QWidget *parent) : QLabel(parent), width(width), height(height) {
    this->setTitle(name);
}

void LevelTitle::onLevelNameUpdated(std::string name)
{
    this->setTitle(name);
}

void LevelTitle::setTitle(const std::string &name)
{
    std::string dim_text = "(" + std::to_string(width) + " X " + std::to_string(height) + ")";
    this->setText((name + " " + dim_text).c_str());
}
