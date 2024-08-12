#ifndef FILE_EVENT_LISTENER_EVENTLISTENER_H
#define FILE_EVENT_LISTENER_EVENTLISTENER_H

#include <string>
#include <memory>

#include "event/event/Event.h"

namespace Mineworld {
    /// @brief Interface for event listeners.
    class EventListener {
    public:
        EventListener(std::string listenerId);

        auto getListenerId() const noexcept -> const std::string&;
        virtual void onEvent(std::shared_ptr<Event> event) = 0;
        virtual ~EventListener() = default;
    protected:
    private:
        std::string listenerId;
        
    };
}

#endif // FILE_EVENT_LISTENER_EVENTLISTENER_H