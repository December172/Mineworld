#include "Event.h"

#include <utility>

namespace Mineworld {
    Event::Event(std::string typeId) noexcept : typeId(std::move(typeId)) {}

    auto Event::getTypeId() const noexcept -> const std::string& {
        return typeId;
    }
}