#include <regex>
#include <thread>
#include <future>

#include "ResourceManager.h"
#include "Mineworld.h"
#include "client/resource/Texture.h"
#include "client/resource/ShaderProgram.h"
#include "exception/IOException.h"
#include "exception/RuntimeException.h"
#include "util/ThreadPool.h"

namespace Mineworld {
    class Mineworld;

    std::function<std::shared_ptr<Resource>(std::shared_ptr<unsigned char>, std::size_t, const std::string&)> textureLoader = [](std::shared_ptr<unsigned char> buffer, std::size_t size, const std::string& uniformId) {
        auto texture = std::make_shared<Texture>(buffer, size, uniformId);
        return texture;
    };

    std::function<std::shared_ptr<Resource>(std::shared_ptr<unsigned char>, std::size_t, const std::string&)> vertexShaderLoader = [](std::shared_ptr<unsigned char> buffer, std::size_t size, const std::string& uniformId) {
        auto source = std::string((char*)buffer.get(), size);
        auto shader = std::make_shared<ShaderProgram>(source, ShaderProgram::Type::vertex, uniformId);
        return shader;
    };

    std::function<std::shared_ptr<Resource>(std::shared_ptr<unsigned char>, std::size_t, const std::string&)> fragmentShaderLoader = [](std::shared_ptr<unsigned char> buffer, std::size_t size, const std::string& uniformId) {
        auto source = std::string((char*)buffer.get(), size);
        auto shader = std::make_shared<ShaderProgram>(source, ShaderProgram::Type::fragment, uniformId);
        return shader;
    };

    ResourceManager::ResourceManager() {
        registerResourceFactory("png", textureLoader);
        registerResourceFactory("vert", vertexShaderLoader);
        registerResourceFactory("frag", fragmentShaderLoader);
    }

    auto ResourceManager::registerResourceFactory(const std::string& extension, std::function<std::shared_ptr<Resource>(std::shared_ptr<unsigned char>, std::size_t, const std::string&)> factory) -> void {
        /// TODO: Should be well-behaved when extension is already registered
        /// Currently, it just simply throw an exception
        if (resourceFactories.find(extension) != resourceFactories.end()) {
            throw std::logic_error(std::string("Resource factory already registered for extension: ") + extension);
        }
        resourceFactories.emplace(extension, factory);
    }

    auto ResourceManager::loadResourcePack(const std::filesystem::path &path) -> const std::string {
        auto pathStr = path.string();
        Mineworld::getLogger().info(std::string("Loading resource pack: ") + pathStr);

        auto status = 0;
        auto *resourcePack = zip_open(pathStr.c_str(), ZIP_RDONLY, &status);
        auto results = std::unordered_map<std::string, std::future<std::shared_ptr<Resource>>>();
        if (resourcePack == nullptr) {
            zip_error_t error;
            zip_error_init_with_code(&error, status);
            const auto* message = zip_error_strerror(&error);
            zip_error_fini(&error);
            throw IOException(std::string("Failed to open resource pack: ") + message);            
        }
        resourcePacks.emplace(pathStr, std::shared_ptr<zip_t>(resourcePack, zip_close));

        std::cmatch match;
        /// TODO: Factory with filter
        auto pattern = std::regex(".*\\.(lua|vert|frag|png|otf|json|ogg)$");
        auto count = zip_get_num_entries(resourcePack, 0);
        auto threadPool = std::make_shared<ThreadPool>(4);
        for (auto i = 0; i < count; i++) {
            const auto* fileName = zip_get_name(resourcePack, i, 0);
            if (fileName == nullptr) {
                /// Something went wrong, skip this file
                continue;
            } 
            if (std::regex_match(fileName, match, pattern)) {
                /// This is a file we want to load
                /// Rename it for uniform id
                auto fileNameStr = std::string(fileName);
                auto extension = fileNameStr.substr(fileNameStr.find_last_of('.') + 1);
                auto fileId = fileNameStr.substr(0, fileNameStr.find_last_of('.'));
                std::replace(fileId.begin(), fileId.end(), '/', '.');
                
                zip_stat_t fileInfo;
                zip_stat_init(&fileInfo);
                zip_file_t* file = zip_fopen_index(resourcePack, i, ZIP_FL_UNCHANGED);
                if (file == nullptr) {
                    /// Something went wrong, skip this file
                    continue;
                }
                zip_stat_index(resourcePack, i, ZIP_FL_UNCHANGED, &fileInfo);
                std::shared_ptr<unsigned char> buffer(new unsigned char[fileInfo.size], std::default_delete<unsigned char[]>());
                auto bytesRead = (zip_uint64_t)zip_fread(file, buffer.get(), fileInfo.size);
                if (bytesRead != fileInfo.size) {
                    /// Something went wrong, skip this file
                    continue;
                }
                zip_fclose(file);
                for (auto& [fileType, factory] : resourceFactories) {
                    if (extension == fileType) {
                        results.emplace(fileId, threadPool->enqueue(factory, buffer, fileInfo.size, fileId));
                        break;
                    }
                }
            }
        }
        for (auto& [uniformId, result] : results) {
            auto resource = result.get();
            if (resource != nullptr) {
                addResource(uniformId, resource);
            }
        }
        return path.filename().string().substr(0, path.filename().string().find_last_of('.'));
    }

    auto ResourceManager::getResource(const std::string& id) -> Resource& {
        auto it = resourceList.find(id);
        if (it == resourceList.end()) {
            throw RuntimeException(std::string("Resource not found: ") + id, Severity::error);
        }
        return *(it->second);
    }

    auto ResourceManager::addResource(const std::string& id, std::shared_ptr<Resource> resource) noexcept -> void {
        resourceList.emplace(id, resource);
    }
}