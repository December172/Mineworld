#include <stdexcept>
#include <algorithm>

#include "EventQueue.h"

namespace Mineworld {
    auto EventQueue::pushEvent(std::shared_ptr<Event> event) -> void {
        eventQueue.push(event);
    }

    auto EventQueue::pollEvent() -> std::shared_ptr<Event> {
        if (eventQueue.empty()) {
            return nullptr;
        }
        return eventQueue.front();
    }

    auto EventQueue::popEvent() -> std::shared_ptr<Event> {
        std::shared_ptr<Event> event = pollEvent();
        eventQueue.pop();
        return event;
    }
    
    auto EventQueue::empty() const noexcept -> bool {
        return eventQueue.empty();
    }

    auto EventQueue::dispatchEvent(std::shared_ptr<Event> event) -> void{
        try {
            auto& listeners = eventListeners.at(event->getTypeId());
            for (auto& listener : listeners) {
                listener->onEvent(event);
            }
        } catch (const std::out_of_range&) {
            // No listeners for this event
            return;
        }
    }

    auto EventQueue::addListener(const std::string& eventId, std::shared_ptr<EventListener> listener) noexcept -> void{
        if (eventListeners.find(eventId) == eventListeners.end()) {
            eventListeners.emplace(eventId, std::vector<std::shared_ptr<EventListener>>());
        }
        eventListeners.at(eventId).push_back(listener);
    }

    auto EventQueue::removeListener(const std::string& eventId, const std::string& listenerId) noexcept -> void {
        if (eventListeners.find(eventId) == eventListeners.end()) {
            return; /// Not found
        }
        auto& listeners = eventListeners.at(eventId);
        auto newEnd = std::remove_if(listeners.begin(), listeners.end(), 
                                    [&](const std::shared_ptr<EventListener>& listener) {
                                        return listener->getListenerId() == listenerId;
                                    });
        if (newEnd != listeners.end()) {
            listeners.erase(newEnd, listeners.end());
        }

    }
}