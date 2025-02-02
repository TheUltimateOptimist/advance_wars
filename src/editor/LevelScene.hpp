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
    LevelScene(const std::string& name, int width, int height, std::vector<uint8_t> tile_ids, const std::string& file_path, QWidget *parent = nullptr);
    static LevelScene* empty(const std::string& name, int width, int height, QWidget *parent = nullptr);
    static LevelScene* fromFile(const std::string& file_path, QWidget *parent = nullptr);
    std::string getName();
    int getWidth();
    int getHeight();
private:
    void onLevelNameUpdated(std::string new_name) override;
    void onLevelWriteRequested(QString file_path) override;
    void onTileEntered(int index) override;  
    void onTileExited(int index) override;
    void onTileClicked(int index) override;
    void onNewTileIdSelected(uint8_t tile_id) override;
    QGraphicsPixmapItem* occupy_tile(int index, uint8_t tile_id);
    uint8_t selected_tile_id;
    std::string name;
    int width;
    int height;
    std::vector<uint8_t> tile_ids;
    std::vector<QGraphicsPixmapItem*> tile_occupants;
    std::string file_path;
};

#endif // LEVELSCENE_HPP