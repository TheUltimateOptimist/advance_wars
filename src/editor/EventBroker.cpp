#include "EventBroker.hpp"

std::vector<EventBroker*> EventBroker::instances = {};

EventBroker::EventBroker() {
    instances.push_back(this);
}

EventBroker::~EventBroker() {
    instances.erase(std::remove(instances.begin(), instances.end(), this), instances.end());
}

/////////////update the level name

void EventBroker::sendLevelNameUpdated(std::string new_name) {
    for (auto instance : instances) {
        instance->onLevelNameUpdated(new_name);
    }
}
void EventBroker::onLevelNameUpdated(std::string new_name) {}

////////////request to write the level

void EventBroker::sendLevelWriteRequested() {
    for (auto instance : instances) {
        instance->onLevelWriteRequested();
    }
}
void EventBroker::onLevelWriteRequested() {}

void EventBroker::sendTileEntered(Tile *tile)
{
    for (auto instance : instances) {
        instance->onTileEntered(tile);
    }
}
void EventBroker::onTileEntered(Tile *tile)
{
}

void EventBroker::sendTileExited(Tile *tile)
{
    for (auto instance : instances) {
        instance->onTileExited(tile);
    }
}
void EventBroker::onTileExited(Tile *tile)
{
}

void EventBroker::sendTileClicked(Tile *tile)
{
    for (auto instance : instances) {
        instance->onTileClicked(tile);
    }
}
void EventBroker::onTileClicked(Tile *tile)
{
}
