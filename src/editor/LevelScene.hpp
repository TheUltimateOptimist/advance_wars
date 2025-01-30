#ifndef LEVELSCENE_HPP
#define LEVELSCENE_HPP

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "EventBroker.hpp"
#include "Tile.hpp"

class LevelScene : public QGraphicsScene, public EventBroker {
public:
    LevelScene(const std::string& name, int width, int height, std::vector<Tile*> tiles, const std::string& file_path, QWidget *parent = nullptr);
    static LevelScene* empty(const std::string& name, int width, int height, QWidget *parent = nullptr);
    static LevelScene* fromFile(const std::string& file_path, QWidget *parent = nullptr);
    std::string getName();
    int getWidth();
    int getHeight();
private:
    void position(Tile* tile, int index);
    void createChildOn(Tile* tile);
    QGraphicsRectItem* createMarkerOn(Tile* tile);
    void onLevelNameUpdated(std::string new_name) override;
    void onLevelWriteRequested() override;
    void onTileEntered(Tile* tile) override;  
    void onTileExited(Tile* tile) override;
    void onTileClicked(Tile* tile) override;
    void onTileSelected(uint8_t id) override;
    Tile* active_tile;
    QGraphicsRectItem* active_tile_marker;
    QGraphicsRectItem* hovered_tile_marker;
    std::string name;
    int width;
    int height;
    std::vector<Tile*> tiles;
    std::string file_path;
};

#endif // LEVELSCENE_HPP