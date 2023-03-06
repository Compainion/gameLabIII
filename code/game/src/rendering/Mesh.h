#pragma once

#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include "tiny_gltf/tiny_gltf.h"

namespace gl3 {
    struct Primitive {
        int mode;
        size_t count;
        int type;
    };

    class Mesh {
    public:
        explicit Mesh(const std::filesystem::path &gltfAssetPath, int meshIndex = 0);

        Mesh(const Mesh &other) = delete;

        Mesh(Mesh &&other) noexcept {
            std::swap(this->VAO, other.VAO);
            std::swap(this->buffers, other.buffers);
            std::swap(this->vaa, other.vaa);
            std::swap(this->primitives, other.primitives);
        }

        void draw() const;

        ~Mesh();

    private:
        tinygltf::Model loadGltf(const std::filesystem::path &gltfAssetPath);

        unsigned int VAO = 0;
        std::map<int, unsigned int> buffers;
        std::map<std::string, unsigned int> vaa = {
                {"POSITION", 0},
                {"NORMAL",   1}
        };
        std::vector<Primitive> primitives;
    };
}