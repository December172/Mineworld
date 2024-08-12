#include "EventListener.h"

namespace Mineworld {
    EventListener::EventListener(std::string listenerId) : listenerId(std::move(listenerId)) {}

    auto EventListener::getListenerId() const noexcept -> const std::string& {
        return listenerId;
    }
}