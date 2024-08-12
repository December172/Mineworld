#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mineworld.h"
#include "Renderer.h"
#include "client/resource/ShaderProgram.h"
#include "client/resource/Texture.h"
#include "exception/RuntimeException.h"

namespace Mineworld {
    class Mineworld;
    /// For testing purposes, we use hardcoded vertices and shader program
    /// Stage 1: Vertices only
    /* Rectangle vertices and indices
    static float vertices[] = {
        -0.5F, 0.5F, 0.0F,  /// Top left
        -0.5F, -0.5F, 0.0F, /// Bottom left
        0.5F, -0.5F, 0.0F,  /// Bottom right
        0.5F, 0.5F, 0.0F    /// Top right
        };
    static GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    */
    /// Stage 2: Vertices with RGB
    /// First three are vertices, last three are RGB color
    /*static float vertices[] = {
        0.5F,  -0.5F, 0.0F, 0.0F, 0.0F, 1.0F,  
        -0.5F, -0.5F, 0.0F, 0.0F, 1.0F, 0.0F, 
        0.0F,   0.5F, 0.0F, 1.0F, 0.0F, 0.0F };
    static GLuint indices[] = {0, 1, 2};
    */
    /// Stage 3: Vertices with texture coordinates
    static float vertices[] = {
            0.5F, 0.5F, 0.0F,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,    /// Top right
            0.5F, -0.5F, 0.0F,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,    /// Bottom right
            -0.5F, -0.5F, 0.0F,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,    /// Bottom left
            -0.5F, 0.5F, 0.0F,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f     /// Top left
        };
    static GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    /// Stage 4: Martix transformation
    static glm::mat4 trans = glm::mat4(1.0F);

    Renderer::Renderer(Window &window) {
        pWindow.reset(&window);
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> pRenderer(SDL_CreateRenderer(window.getHandle(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
        if (pRenderer == nullptr) {
            throw RuntimeException(std::string("Failed to create renderer! Message: ") + std::string(SDL_GetError()), Severity::fatal);
        }
        /// OpenGL initialize

        glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
        glViewport(0, 0, Window::WIDTH, Window::HEIGHT);
        opaqueness = 0.2F;
        angle = 45.0F;
        /// Create VBO, EBO, VAO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        trans = glm::translate(trans, glm::vec3(0.0F, 0.0F, 0.0F));

        /// Current we use hardcoded resource uniform id
        auto &vertexShader = dynamic_cast<ShaderProgram &>(Mineworld::getInstance()
                                                                      .getResourceManager()
                                                                      .getResource("mineworld.shaders.vertex"));
        auto &fragmentShader = dynamic_cast<ShaderProgram &>(Mineworld::getInstance()
                                                                         .getResourceManager()
                                                                         .getResource("mineworld.shaders.fragment1"));
        auto program = std::make_shared<ShaderProgram>(vertexShader, fragmentShader, "mineworld.shaders.program");
        Mineworld::getInstance().getResourceManager().addResource("mineworld.shaders.program", program);
        Mineworld::getLogger().info("Renderer loaded.");
    }

    auto Renderer::setRotation(GLfloat angle) -> void {
        this->angle = angle;
        trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0F, 0.0F, 1.0F));
    }

    auto Renderer::getRotation() const -> GLfloat {
        return angle;
    }
    
    void Renderer::render() {
        auto& texture1 = dynamic_cast<Texture &>(Mineworld::getInstance()       
                                                           .getResourceManager()
                                                           .getResource("mineworld.textures.tile.catous"));
        auto& texture2 = dynamic_cast<Texture &>(Mineworld::getInstance()
                                                           .getResourceManager()
                                                           .getResource("mineworld.textures.tile.spawner"));
        auto& shaderProgram = dynamic_cast<ShaderProgram &>(Mineworld::getInstance()
                                                 .getResourceManager()
                                                 .getResource("mineworld.shaders.program"));
        
        /// Start rendering
        glClear(GL_COLOR_BUFFER_BIT);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shaderProgram.use();
        texture1.bind(0);
        texture2.bind(1);
        shaderProgram.setInt("texture1", 0);
        shaderProgram.setInt("texture2", 1);
        shaderProgram.setFloat("opaqueness", opaqueness);
        shaderProgram.setMat4("transform", trans);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /// End rendering
        SDL_GL_SwapWindow(pWindow.get()->getHandle());
    }

} // namespace Mineworld
