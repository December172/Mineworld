#include <mutex>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Texture.h"
#include "exception/RuntimeException.h"
#include "Mineworld.h"

namespace Mineworld {

    Texture::Texture(std::shared_ptr<unsigned char> data, std::size_t size, std::string id) : Resource(std::move(id)) {
        SDL_GLContext sharedContext = Mineworld::getInstance().getWindow().getSharedGLContext();
        std::unique_lock<std::mutex> lock(Mineworld::getInstance().getWindow().getSharedGLContextMutex());
        if (SDL_GL_MakeCurrent(Mineworld::getInstance().getWindow().getHandle(), sharedContext) < 0) {
            throw RuntimeException("Failed to make shared OpenGL context current! Message: " + std::string(SDL_GetError()), Severity::fatal);
        }
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            throw RuntimeException(std::string("Failed to initialize GLEW in initializating shader program!"), Severity::fatal);
        }   
        int w = 0;
        int h = 0;
        int channels = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* image = stbi_load_from_memory(data.get(), size, &w, &h, &channels, STBI_rgb_alpha);
        if (!image) {
            throw RuntimeException(std::string("Failed to load texture:") + id, Severity::error);
        }
        width = w;
        height = h;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(image);
        glBindTexture(GL_TEXTURE_2D, 0);
        SDL_GL_MakeCurrent(Mineworld::getInstance().getWindow().getHandle(), nullptr);
    }

    auto Texture::getTextureId() const noexcept -> GLuint {
        return textureId;
    }

    auto Texture::unload() -> void {
        glDeleteTextures(1, &textureId);
    }

    auto Texture::getHeight() const noexcept -> int {
        return height;
    }

    auto Texture::getWidth() const noexcept -> int {
        return width;
    }

    auto Texture::bind(unsigned int textureUnit) const -> void {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    Texture::~Texture() {
        unload();
    }
}
