#ifndef FILE_CLIENT_RESOURCE_SHADERPROGRAM_H
#define FILE_CLIENT_RESOURCE_SHADERPROGRAM_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Resource.h"

namespace Mineworld {
    /// @brief Represents a shader program resource
    class ShaderProgram : public Resource {
        public:
            /// @brief Shader program type
            /// @note 'program' type should not be used directly, 
            ///        use 'vertex' and 'fragment' instead
            enum class Type { vertex, fragment, program };
            /// @brief Vertex and fragment shaders constructor
            ShaderProgram(std::string content, ShaderProgram::Type shaderType, std::string id);

            /// @brief Links vertex and fragment shaders into a program
            ShaderProgram(ShaderProgram& vertexShader, ShaderProgram& fragmentShader, const std::string& id);

            /// @brief Get program id
            /// @return program id of this shader program
            auto getProgramId() const noexcept -> unsigned int;

            /// @brief Get shader type
            /// @return shader type of this shader program
            auto getType() const noexcept -> const ShaderProgram::Type&;

            /// Uniform setters
            auto setInt(const std::string& name, int value) const -> void;
            auto setFloat(const std::string& name, float value) const -> void;
            auto setVec2(const std::string& name, const glm::vec2& value) const -> void;
            auto setVec3(const std::string& name, const glm::vec3& value) const -> void;
            auto setMat4(const std::string& name, const glm::mat4& value) const -> void;

            /// @brief Use this shader program for rendering
            auto use() const -> void;

            ~ShaderProgram() override;
        protected:
            void unload() override;
        private:
            /// To distinguish between uniform id
            unsigned int programId;
            std::string source;
            ShaderProgram::Type type;

            void compileShader();
    };
}
#endif // FILE_CLIENT_RESOURCE_SHADERPROGRAM_H
