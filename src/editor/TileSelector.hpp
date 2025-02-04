/**
* TileSelector.cpp
*
* @date 28.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de) skeleton
* @author Nils Jonathan Friedrich Eckardt implementation
*/

#pragma once

#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include "SpriteProvider.hpp"
#include <QGridLayout>

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