/**
* TileSelector.cpp
*
* @date 28.01.2025
* @author Nils Jonathan Friedrich Eckardt implementation
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de) skeleton
*/

#pragma once

#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

#include "SpriteProvider.hpp"

namespace editor
{

class TileSelector : public QScrollArea {
public:
    TileSelector(QWidget *parent = nullptr);
    
private:
    QLabel *createNewLabel(QWidget* parent, const char *text);

    template<typename T>
    void sectionLayout(QGridLayout*& layout, int usedIdCounter, QWidget* parent, T id);
    template<typename T, typename... Rest>
    void sectionLayout(QGridLayout*& layout, int usedIdCounter, QWidget* parent, T id, Rest...ids);
    template<typename... T>
    QGridLayout* creatSectionLayout(QWidget* parent, T... ids);
};

} // namespace editor