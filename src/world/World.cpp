#include "World.h"

namespace Mineworld {
    World::World(const std::string& worldName) {
        // TODO: Implement World::World()
        // This constructor should initialize the world with the given name.
    }
    
    World::World(const std::filesystem::path& worldPath) {
        // TODO: Implement world loading from file
    }

    //auto World::getCurrentDimension() -> Dimension&{
    //    return *currentDimension;
    //}

    auto World::save() -> void {
        /// TODO: Implement world saving to file using levelDB
    }

    World::~World() {
        save();
    }
}