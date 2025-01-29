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
