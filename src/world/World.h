#ifndef FILE_WORLD_WORLD_H
#define FILE_WORLD_WORLD_H

#include <map>
#include <memory>
#include <string>
#include <filesystem>

//#include "Dimension.h"

namespace Mineworld {
    class World {
    public:
        /// @brief Constructor for new World object
        /// @param name name of the world
        World(const std::string& worldName);
        /// @brief Constructor for loading existing World object from file
        /// @param path path to the world file
        World(const std::filesystem::path& worldPath);
        World(const World& other) = delete; 
        /// @brief  Get the current dimension of the world
        /// @param id Dimension ID
        /// @return current dimension of the world
  //      auto getCurrentDimension(std::string id) const noexcept -> Dimension&;
        auto render() -> void; 
        auto tick() -> void;
        auto save() -> void;

        ~World();
    protected:
    private:
    //    std::map<std::string, std::shared_ptr<Dimension>> dimensions;
    //    std::shared_ptr<Dimension> currentDimension;
    };
}

#endif // FILE_WORLD_WORLD_H