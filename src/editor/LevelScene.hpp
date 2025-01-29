#ifndef LEVELSCENE_HPP
#define LEVELSCENE_HPP

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <tuple>
#include "EventBroker.hpp"

class LevelScene : public QGraphicsScene, public EventBroker {
public:
    LevelScene(const std::string& name, int width, int height, uint8_t* tiles, QWidget *parent = nullptr);
    ~LevelScene();
    static LevelScene* empty(const std::string& name, int width, int height, QWidget *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    void onLevelNameUpdated(std::string new_name) override;
    void onLevelWriteRequested() override;
    std::string name;
    int width;
    int height;
    uint8_t* tiles;
};

#endif // LEVELSCENE_HPP