#include "EventBroker.hpp"

namespace editor
{

std::vector<EventBroker*> EventBroker::instances = {};

EventBroker::EventBroker() {
    instances.push_back(this);
}

EventBroker::~EventBroker() {
    instances.erase(std::remove(instances.begin(), instances.end(), this), instances.end());
}

void EventBroker::send(std::function<void(EventBroker *)> callback)
{
    for (EventBroker* instance : instances) {
        callback(instance);
    }
}

} // namespace editor