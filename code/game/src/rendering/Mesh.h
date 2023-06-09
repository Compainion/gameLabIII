#pragma once

#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include "tiny_gltf/tiny_gltf.h"
#include "glm/fwd.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace gl3 {
    struct Primitive {
        int mode;
        size_t count;
        int type;
    };

    struct loadedTexture{
        std::string name;
        unsigned int texID;
    };

    class Mesh {
    public:
        explicit Mesh(const std::filesystem::path &gltfAssetPath, int meshIndex = 0, int instancingAmount = 1, glm::mat4 modelMatrices[] = {});

        Mesh(const Mesh &other) = delete;

        Mesh(Mesh &&other) noexcept {
            std::swap(this->VAO, other.VAO);
            std::swap(this->buffers, other.buffers);
            std::swap(this->vaa, other.vaa);
            std::swap(this->primitives, other.primitives);
            std::swap(this->texture, other.texture);
            std::swap(this->instancingAmount, other.instancingAmount);
        }

        void draw() const;

        ~Mesh();

    private:
        tinygltf::Model loadGltf(const std::filesystem::path &gltfAssetPath);

        static std::vector<loadedTexture> loadedTextures;
        static unsigned int GetTextureID(const std::string &textureName);

        unsigned int VAO = 0;
        unsigned int texture = 0;
        unsigned int instancingAmount = 1;
        std::map<int, unsigned int> buffers;
        std::map<std::string, unsigned int> vaa = {
                {"POSITION", 0},
                {"NORMAL",   1},
                {"TEXCOORD_0", 2}
        };
        std::vector<Primitive> primitives;
    };
}