#ifndef FILE_CLIENT_RENDERER_RENDERER_H
#define FILE_CLIENT_RENDERER_RENDERER_H

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#define GL_GLEXT_PROTOTYPES
#include <GLEW/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include "client/Window.h"
#include "client/resource/ShaderProgram.h"

namespace Mineworld {
    class Window;
    class Renderer {
        public:
            Renderer(Window& window);
            void render();
            /// for testing
            auto getRotation() const -> GLfloat;
            auto setRotation(GLfloat angle) -> void;
            GLfloat opaqueness;

        protected:
        private:
            GLuint VAO;
            GLuint VBO;
            GLuint EBO;
            GLfloat angle;
            std::shared_ptr<Window> pWindow;
            std::shared_ptr<SDL_Renderer> pRenderer;
    };
}

#endif // FILE_CLIENT_RENDERER_RENDERER_H
