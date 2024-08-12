#ifndef FILE_CLIENT_WINDOW_H
#define FILE_CLIENT_WINDOW_H

#include <string>
#include <memory>
#include <mutex>

#include <SDL2/SDL_video.h>

namespace Mineworld {
    class Window final {
        public:
            static const int WIDTH  = 640;
            static const int HEIGHT = 480;
            Window();

            /// Getters
            auto getTitle() const                -> const std::string;
            auto getHandle() const               -> SDL_Window*;
            auto getGLContext() const            -> SDL_GLContext;
            auto getSharedGLContext() const      -> SDL_GLContext;
            auto getSharedGLContextMutex() const -> std::mutex&;

            /// Setters
            auto setTitle(const std::string& title) -> void;
        protected:
        private:
            std::shared_ptr<SDL_Window> pWindow;
            SDL_GLContext glContext;
            mutable std::mutex sharedGLContextMutex;
            SDL_GLContext sharedGLContext;
    };
}
#endif // FILE_CLIENT_WINDOW_H
