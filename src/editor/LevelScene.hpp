#ifndef LEVELSCENE_HPP
#define LEVELSCENE_HPP

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <tuple>

class LevelScene : public QGraphicsScene {
public:
    LevelScene(int width, int height, uint8_t* tiles, QWidget *parent = nullptr);
    ~LevelScene();
    static LevelScene* empty(int width, int height, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    int width;
    int height;
    uint8_t* tiles;
};

#endif // LEVELSCENE_HPP