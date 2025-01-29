#ifndef LEVELSCENE_HPP
#define LEVELSCENE_HPP

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <tuple>

class LevelScene : public QGraphicsScene {
public:
    LevelScene(int width, int height, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    int width;
    int height;
};

#endif // LEVELSCENE_HPP