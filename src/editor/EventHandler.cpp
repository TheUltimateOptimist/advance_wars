/**
* EventHandler.cpp
*
* @date 29.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "EventHandler.hpp"

namespace editor
{

std::vector<EventHandler*> EventHandler::instances = {};

EventHandler::EventHandler()
{
    // register event handler
    instances.push_back(this);
}

EventHandler::~EventHandler()
{
    // deregister event handler
    instances.erase(std::remove(instances.begin(), instances.end(), this), instances.end());
}

void EventHandler::send(std::function<void(EventHandler*)> callback)
{
    // Execute the provided callaback on every EventHandler instance.
    for (EventHandler* instance : instances)
    {
        callback(instance);
    }
}

} // namespace editor