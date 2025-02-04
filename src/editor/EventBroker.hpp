/**
* EventBroker.cpp
*
* @date 29.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <QString>

namespace editor
{

class Tile;
class EventBroker {
public:
    EventBroker();
    virtual ~EventBroker();

    static void send(std::function<void(EventBroker*)> callback);

    virtual void onLevelNameUpdated(std::string new_name){};

    virtual void onLevelWriteRequested(QString file_path){};

    virtual void onTileEntered(int index){};

    virtual void onTileExited(int index){};

    virtual void onTileClicked(int index){};

    virtual void onNewTileIdSelected(uint8_t tile_id){};

    virtual void onZoomed(double delta){}

private:
    static std::vector<EventBroker*> instances ;
};

} // namespace editor