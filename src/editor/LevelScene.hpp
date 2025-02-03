#pragma once

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "EventBroker.hpp"
#include "Tile.hpp"

namespace editor
{

class LevelScene : public QGraphicsScene, public EventBroker {
public:
    LevelScene(const std::string& name, int width, int height, std::vector<uint8_t> tile_ids, const std::string& file_path, QWidget *parent = nullptr);
    static LevelScene* empty(const std::string& name, int width, int height, QWidget *parent = nullptr);
    static LevelScene* fromFile(const std::string& file_path, QWidget *parent = nullptr);
    std::string getName();
    int getWidth();
    int getHeight();
private:
    bool is_border(int index);
    bool is_water_tile(uint8_t id);
    void onLevelNameUpdated(std::string new_name) override;
    void onLevelWriteRequested(QString file_path) override;
    void onTileEntered(int index) override;  
    void onTileExited(int index) override;
    void onTileClicked(int index) override;
    void onNewTileIdSelected(uint8_t tile_id) override;
    QGraphicsPixmapItem* occupy_tile(int index, uint8_t tile_id);
    uint8_t m_selected_tile_id;
    std::string m_name;
    int m_width;
    int m_height;
    std::vector<uint8_t> m_tile_ids;
    std::vector<QGraphicsPixmapItem*> m_tile_occupants;
    std::string m_file_path;
};

} // namespace editor