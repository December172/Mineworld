#ifndef FILE_EVENT_EVENTQUEUE_H
#define FILE_EVENT_EVENTQUEUE_H

#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

#include "event/event/Event.h"
#include "event/listener/EventListener.h"

namespace Mineworld {
    class EventQueue {
    public:
        EventQueue() = default;
        auto pushEvent(std::shared_ptr<Event> event) -> void;
        /// @brief Poll the event queue and return the last event in the queue.
        /// @return Pointer to the last event in the queue.
        /// @note This function won't remove the event from the queue.
        /// @throw std::out_of_range if the queue is empty.
        auto pollEvent() -> std::shared_ptr<Event>;
        /// @brief Pop the last event in the queue.
        /// @return Pointer to the last event in the queue.
        /// @note This function will remove the event from the queue.
        /// @throw std::out_of_range if the queue is empty.
        auto popEvent() -> std::shared_ptr<Event>;
        auto empty() const noexcept -> bool;
        auto dispatchEvent(std::shared_ptr<Event> event) -> void;
        auto addListener(const std::string& eventId, std::shared_ptr<EventListener> listener) noexcept -> void;
        auto removeListener(const std::string& eventId, const std::string& listenerId) noexcept -> void;
    protected:
    private:
        std::queue<std::shared_ptr<Event>> eventQueue;
        /// @brief Map of event listeners.
        /// @note The key is the event ID, and the value is a vector of event listeners.
        std::unordered_map<std::string, std::vector<std::shared_ptr<EventListener>>> eventListeners;
    };
}

#endif // FILE_EVENT_EVENTQUEUE_H