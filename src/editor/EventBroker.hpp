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

    virtual void onTileEntered(Tile* tile){};

    virtual void onTileExited(Tile* tile){};

    virtual void onTileClicked(Tile* tile){};

    virtual void onTileSelected(uint8_t id){};

private:
    static std::vector<EventBroker*> instances ;
};