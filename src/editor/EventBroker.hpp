#pragma once
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>

class Tile;
class EventBroker {
public:
    EventBroker();
    virtual ~EventBroker();

    static void send(std::function<void(EventBroker*)> callback);

    virtual void onLevelNameUpdated(std::string new_name){};

    virtual void onLevelWriteRequested(){};

    virtual void onTileEntered(int index){};

    virtual void onTileExited(int index){};

    virtual void onTileClicked(int index){};

    virtual void onNewTileIdSelected(uint8_t tile_id){};

private:
    static std::vector<EventBroker*> instances ;
};