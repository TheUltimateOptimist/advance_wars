#pragma once
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>

class Tile;
class EventBroker {
public:
    EventBroker();
    virtual ~EventBroker();

    void sendLevelNameUpdated(std::string new_name);
    virtual void onLevelNameUpdated(std::string new_name);

    void sendLevelWriteRequested();
    virtual void onLevelWriteRequested();

    void sendTileEntered(Tile* tile);
    virtual void onTileEntered(Tile* tile);

    void sendTileExited(Tile* tile);
    virtual void onTileExited(Tile* tile);

    void sendTileClicked(Tile* tile);
    virtual void onTileClicked(Tile* tile);

    void sendTileSelected(uint8_t id);
    virtual void onTileSelected(uint8_t id);

private:
    static std::vector<EventBroker*> instances ;
};