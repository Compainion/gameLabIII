//
// Created by Megaport on 06/02/2023.
//

#pragma once

#include <string>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "filesystem"

namespace fs = std::filesystem;

namespace gl3 {
    class Shader {

    public:
        Shader(const fs::path &vertexShaderPath, const fs::path &fragmentShaderPath);

        std::string readText(const fs::path &filePath);

        // Delete copy constructor
        Shader(const Shader &shader) = delete;

        // Explicit move constructor
        Shader(Shader &&other) noexcept {
            std::swap(this->shaderProgram, other.shaderProgram);
            std::swap(this->vertexShader, other.vertexShader);
            std::swap(this->fragmentShader, other.fragmentShader);
        }

        virtual ~Shader();

        void setMatrix(const std::string &uniformName, glm::mat4 matrix) const;

        void setVector4(const std::string &uniformName, glm::vec4 vector) const;

        void setVector3(const std::string &uniformName, glm::vec3 vector) const;

        void setFloat(const std::string &uniformName, float number) const;

        void setInt(const std::string &uniformName, int number) const;

        void use() const;

    private:
        unsigned int loadAndCompileShader(GLuint shaderType, const fs::path &shaderPath);

        unsigned int shaderProgram = 0;
        unsigned int vertexShader = 0;
        unsigned int fragmentShader = 0;
    };
}