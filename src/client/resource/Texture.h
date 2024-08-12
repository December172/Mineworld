#ifndef FILE_CLIENT_RESOURCE_TEXTURE_H
#define FILE_CLIENT_RESOURCE_TEXTURE_H

#include <memory>

#define GL_GLEXT_PROTOTYPES
#include <GLEW/glew.h>

#include "Resource.h"

namespace Mineworld {
    class Texture : public Resource {
        public:
            Texture(std::shared_ptr<unsigned char> data, std::size_t size, std::string id);
            /// @brief Access width
            /// @return The current value of width
            auto getWidth() const noexcept -> int;
            /// @brief Access height
            /// @return The current value of height
            auto getHeight() const noexcept -> int;

            /// @brief Get the texture ID
            /// @return The texture ID
            /// @note This function should only be called from the renderer
            auto getTextureId() const noexcept -> GLuint;

            /// @brief Bind the texture to the current OpenGL context            
            /// @note This function should only be called from the renderer 
            auto bind(unsigned int textureUnit) const -> void;

            ~Texture() override;
        protected:
            void unload() override;
        private:
            //std::shared_ptr<SDL_Texture> rawTexture;
            int width;
            int height;
            GLuint textureId;
    };
}
#endif // FILE_CLIENT_RESOURCE_TEXTURE_H
