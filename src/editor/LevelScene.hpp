#ifndef LEVELSCENE_HPP
#define LEVELSCENE_HPP

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include "EventBroker.hpp"

class LevelScene : public QGraphicsScene, public EventBroker {
public:
    LevelScene(const std::string& name, int width, int height, std::vector<uint8_t> tiles, const std::string& file_path, QWidget *parent = nullptr);
    static LevelScene* empty(const std::string& name, int width, int height, QWidget *parent = nullptr);
    static LevelScene* fromFile(const std::string& file_path, QWidget *parent = nullptr);
    std::string getName();
    int getWidth();
    int getHeight();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    void onLevelNameUpdated(std::string new_name) override;
    void onLevelWriteRequested() override;
    std::string name;
    int width;
    int height;
    std::vector<uint8_t> tiles;
    std::string file_path;
};

#endif // LEVELSCENE_HPP