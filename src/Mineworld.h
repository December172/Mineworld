#ifndef FILE_MINEWORLD_H
#define FILE_MINEWORLD_H

#include <memory>
#include <string>

#include "event/EventQueue.h"
#include "client/renderer/Renderer.h"
#include "client/resource/ResourceManager.h"
#include "util/Logger.h"

namespace Mineworld {
    /// @brief The main class of the game
    class Mineworld {
        public:
            const static std::string NAME;
            const static std::string VERSION;

            /// @brief Get the logger instance
            static auto getLogger() -> Logger&;      
            /// @brief Get the instance of Mineworld
            static auto getInstance() -> Mineworld&; 

            /// @brief Get the resource manager instance
            auto getResourceManager() -> ResourceManager&;

            /// @brief Get the event queue instance
            auto getEventQueue() -> EventQueue&;

            /// @brief Get the window instance
            auto getWindow() -> Window&;

            /// @brief Get the renderer instance
            auto getRenderer() -> Renderer&;

            /// @brief Run the game
            /// @note Should not be called from outside the class
            auto run() -> int;

            ~Mineworld();
        protected:
        private:
            bool running = false;
            /// Pointer to the window also shared with the renderer
            std::shared_ptr<Window> window;
            std::unique_ptr<Renderer> renderer;
            std::unique_ptr<ResourceManager> resourceManager;
            std::unique_ptr<EventQueue> eventQueue;

            static std::unique_ptr<Logger> logger;
            static std::unique_ptr<Mineworld> theMineworld;

            auto eventLoop() -> void;
            auto init() -> void;
            
            /// Singleton implementation
            Mineworld() = default;
            Mineworld(const Mineworld&) = delete;
            Mineworld& operator=(const Mineworld&) = delete;
    };

}
#endif // FILE_MINEWORLD_H
