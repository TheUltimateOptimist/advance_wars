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
    instances.push_back(this);
}

EventHandler::~EventHandler()
{
    instances.erase(std::remove(instances.begin(), instances.end(), this), instances.end());
}

void EventHandler::send(std::function<void(EventHandler*)> callback)
{
    for (EventHandler* instance : instances)
    {
        callback(instance);
    }
}

} // namespace editor