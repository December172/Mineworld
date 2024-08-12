#include <GLEW/glew.h>

#include <mutex>

#include "Window.h"
#include "exception/RuntimeException.h"

namespace Mineworld {
    Window::Window() {
        SDL_ClearError();

        pWindow = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow("Mineworld",
                                       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                       WIDTH, HEIGHT,
                                       SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS |
                                       SDL_WINDOW_KEYBOARD_GRABBED),
                      SDL_DestroyWindow);
        if (!pWindow.get()) {
            throw RuntimeException(std::string("Failed to create window!, Message: ") + std::string(SDL_GetError()), Severity::fatal);
        }

        glContext = SDL_GL_CreateContext(pWindow.get());
        if (!glContext) {
            throw RuntimeException(std::string("Failed to create OpenGL context!, Message: ") + std::string(SDL_GetError()), Severity::fatal);
        }

        GLenum msg = glewInit();
        if (msg != GLEW_OK) {
            throw RuntimeException(std::string("Failed to initialize GLEW!, Message: ") + std::string(reinterpret_cast<const char*>(glewGetErrorString(msg))), Severity::fatal);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
        {
            std::unique_lock<std::mutex> lock(sharedGLContextMutex);
            sharedGLContext = SDL_GL_CreateContext(pWindow.get());
            if (!sharedGLContext) {
                throw RuntimeException(std::string("Failed to create shared OpenGL context!, Message: ") + std::string(SDL_GetError()), Severity::fatal);
            }
        }
        SDL_GL_MakeCurrent(pWindow.get(), glContext);
    }

    auto Window::setTitle(const std::string& title) -> void {
        SDL_SetWindowTitle(pWindow.get(), title.data());
    }
    
    /// This should not return a reference, but a copy of the title string.
    auto Window::getTitle() const -> const std::string {
        return std::string(SDL_GetWindowTitle(pWindow.get()));
    }

    auto Window::getHandle() const -> SDL_Window* {
        return pWindow.get();
    }

    auto Window::getGLContext() const -> SDL_GLContext {
        return glContext;
    }

    auto Window::getSharedGLContext() const -> SDL_GLContext {
        std::unique_lock<std::mutex> lock(sharedGLContextMutex);
        return sharedGLContext;
    }

    auto Window::getSharedGLContextMutex() const -> std::mutex& {
        return sharedGLContextMutex;
    }
}
