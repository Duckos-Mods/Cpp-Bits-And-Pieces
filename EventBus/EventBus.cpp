#include "EventBus.h"



// Overloaded version of addEventListener for functions without a Class
void EventBus::addEventListener(std::function<void()> function, std::string eventName, bool loop)
{
    auto Wrapper = [function]() { function(); };
    auto emd = eventMetaData(Wrapper, loop);
    _eventListeners.insert({ eventName, emd });
}

// Process all events waiting in the event queue
void EventBus::tick()
{
    // Iterate over each eventName in _allWaitingEvents
    for (auto& eventName : _allWaitingEvents)
    {
        // Search for the eventName in _eventListeners and fire the event if found
        searchForEventAndFire(eventName);
    }

    // Clear the _allWaitingEvents vector
    _allWaitingEvents.clear();
    _allWaitingEvents.resize(0);
}


void EventBus::pushBackEventListener(DataPack dataPair)
{
    auto location = _eventListeners.find(dataPair.index);
    if (location == _eventListeners.end())
    {
        _eventListeners.insert({ dataPair.index, eventMetaData(dataPair.functPair.second, dataPair.functPair.first)});
    }
    else
    {
        // Current Event Data
        auto& CED = _eventListeners.at(dataPair.index);

        CED.addFunct(dataPair.functPair);

    }
}