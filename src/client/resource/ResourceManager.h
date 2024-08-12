#ifndef FILE_CLIENT_RESOURCE_RESOURCEMANAGER_H
#define FILE_CLIENT_RESOURCE_RESOURCEMANAGER_H

#include <unordered_map>
#include <filesystem>
#include <memory>
#include <string>
#include <functional>

#include <libzip/zip.h>

#include "client/resource/Resource.h"

namespace Mineworld {
    class ResourceManager final {
        public:
            
            ResourceManager();
            ~ResourceManager() = default;

            /// @brief  Load a resource pack.
            /// @return ID of the resource pack.
            /// @throws Mineworld::IOException if the resource pack cannot be loaded.
            /// @throws Mineworld::RuntimeException if the resource inside the resource pack is invalid.
            auto loadResourcePack(const std::filesystem::path& path) -> const std::string; 
            
            /// @brief  Get a resource by id.
            /// @note   Use dynamic_cast to convert the resource to the desired type.
            /// @return The resource with the given id.
            /// @throws Mineworld::RuntimeException if the resource is not found.
            auto getResource(const std::string& id) -> Resource&;
            
            /// @brief          Register a resource factory for a specific type of resource to allow more custom resource types.
            /// @param  type    The type of resource to register the factory for.
            /// @param  factory The factory function to create the resource.
            /// @details        The factory function takes the resource id, the raw resource data and its size as parameters and returns a shared pointer to the resource.
            auto registerResourceFactory(const std::string& type, std::function<std::shared_ptr<Resource>(std::shared_ptr<unsigned char>, std::size_t, const std::string&)> factory) -> void;

            /// @brief Add a resource to the resource list.
            auto addResource(const std::string& id, std::shared_ptr<Resource> resource) noexcept -> void;
        protected:
        private:
            std::unordered_map<std::string, std::shared_ptr<zip_t>> resourcePacks;
            std::unordered_map<std::string, std::shared_ptr<Resource>> resourceList;
            /// @brief   Map of different resource types factories
            /// @details The factory function takes the resource id, the raw resource data and its size as parameters and returns a shared pointer to the resource.
            /// @note    The resource type is determined by the file extension of the resource id.
            std::unordered_map<std::string, std::function<std::shared_ptr<Resource>(std::shared_ptr<unsigned char>, std::size_t, const std::string&)>> resourceFactories;
    };
}

#endif // FILE_CLIENT_RESOURCE_RESOURCEMANAGER_H
