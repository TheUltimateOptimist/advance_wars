#pragma once
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>

class EventBroker {
public:
    EventBroker();
    virtual ~EventBroker();

    void sendLevelNameUpdated(std::string new_name);
    virtual void onLevelNameUpdated(std::string new_name);

    void sendLevelWriteRequested();
    virtual void onLevelWriteRequested();

private:
    static std::vector<EventBroker*> instances ;
};