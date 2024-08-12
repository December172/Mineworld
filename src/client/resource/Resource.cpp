#include "Resource.h"

namespace Mineworld {
    Resource::Resource(std::string id) : uniformID(std::move(id)) {};

    auto Resource::getUniformID() const noexcept -> const std::string & {
        return uniformID;
    }
}
