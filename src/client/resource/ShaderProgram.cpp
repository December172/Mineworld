#include <exception>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <utility>

#define GL_GLEXT_PROTOTYPES
#include <GLEW/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#include "ShaderProgram.h"
#include "util/Util.h"
#include "util/Logger.h"
#include "exception/RuntimeException.h"
#include "exception/IOException.h"
#include "Mineworld.h"

namespace Mineworld {
    ShaderProgram::ShaderProgram(std::string content, ShaderProgram::Type shaderType, std::string id) : Resource(std::move(id)), source(std::move(content)), type(std::move(shaderType)) {
        SDL_GLContext sharedContext = Mineworld::getInstance().getWindow().getSharedGLContext();
        std::unique_lock<std::mutex> lock(Mineworld::getInstance().getWindow().getSharedGLContextMutex());
        if (SDL_GL_MakeCurrent(Mineworld::getInstance().getWindow().getHandle(), sharedContext) < 0) {
            throw RuntimeException("Failed to make shared OpenGL context current! Message: " + std::string(SDL_GetError()), Severity::fatal);
        }
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            throw RuntimeException(std::string("Failed to initialize GLEW in initializating shader program!"), Severity::fatal);
        }   
        switch (type) {
        case ShaderProgram::Type::vertex:
            programId = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderProgram::Type::fragment:
            programId = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        default:
            throw new std::invalid_argument("Invalid shader type.");
            break;
        }
        compileShader();
        SDL_GL_MakeCurrent(Mineworld::getInstance().getWindow().getHandle(), nullptr);
    }

    ShaderProgram::ShaderProgram(ShaderProgram &vertexShader, ShaderProgram &fragmentShader, const std::string &id) : Resource(id), programId(glCreateProgram()), type(ShaderProgram::Type::program) {
        glAttachShader(programId, vertexShader.getProgramId());
        glAttachShader(programId, fragmentShader.getProgramId());
        glLinkProgram(programId);

        /// Error handling
        int success = 1;
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            int length;
            glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &length);
            char* message = new char[length];
            glGetShaderInfoLog(programId, length, &length, message);
            std::string errorMessage = "Shader program " + this->getUniformID() 
                                                         + "with vertex shader: "
                                                         + vertexShader.getUniformID() 
                                                         + " and fragment shader: "
                                                         + fragmentShader.getUniformID()
                                                         + " link failed: " 
                                                         + message;
            delete[] message;
            throw RuntimeException(errorMessage, Severity::error);
        }
        /// Don't delete used shaders, as might be used by other programs.
    }

    void ShaderProgram::unload() {
        glDeleteShader(programId);
    }

    auto ShaderProgram::getProgramId() const noexcept -> GLuint {
        return programId;
    }

    auto ShaderProgram::getType() const noexcept -> const ShaderProgram::Type& {
        return type;
    }

    void ShaderProgram::compileShader() {
        const char* sourceCStr = source.c_str();
        glShaderSource(programId, 1, &sourceCStr, nullptr);
        glCompileShader(programId);

        int success = 0;
        glGetShaderiv(programId, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            int length = 0;
            glGetShaderiv(programId, GL_INFO_LOG_LENGTH, &length);
            char* message = new char[length];
            glGetShaderInfoLog(programId, length, &length, message);
            std::string errorMessage = "Shader program " + this->getUniformID() + " compile failed: " + message;
            delete[] message;
            throw RuntimeException(errorMessage, Severity::error);
        }
    }
    
    auto ShaderProgram::setFloat(const std::string& name, float value) const -> void {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform1f(location, value);
    }

    auto ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const -> void {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform2fv(location, 1, glm::value_ptr(value));
    }

    auto ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const -> void {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    auto ShaderProgram::setInt(const std::string& name, int value) const -> void {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniform1i(location, value);
    }

    auto ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const-> void {
        GLint location = glGetUniformLocation(programId, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    auto ShaderProgram::use() const -> void {
        glUseProgram(programId);
    }

    ShaderProgram::~ShaderProgram() {
        unload();
    }
}
