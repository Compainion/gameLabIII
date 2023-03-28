#include "Mesh.h"
#include <iostream>
#include "glad/glad.h"
#include "../Assets.h"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))
namespace gl3 {

    std::vector<loadedTexture> Mesh::loadedTextures;

    Mesh::Mesh(const std::filesystem::path &gltfAssetPath, int meshIndex, int instancingAmount, glm::mat4 modelMatrices[]) {
        Mesh::instancingAmount = instancingAmount;
        auto assetPath = resolveAssetPath(gltfAssetPath);
        auto model = loadGltf(assetPath);

        if (model.meshes.size() <= meshIndex) {
            throw std::runtime_error("[mesh] model does not contain requested mesh: " + gltfAssetPath.string());
        }

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        std::map<int, unsigned int> vbos;
        for (size_t i = 0; i < model.bufferViews.size(); ++i) {
            const auto &bufferView = model.bufferViews[i];
            const auto &buffer = model.buffers[bufferView.buffer];
            if (bufferView.target == 0) {
                //TODO: Remove maybe?!
                //std::cerr << "[mesh] bufferView.target is zero, drawArrays not supported: " << gltfAssetPath.string()
                //          << std::endl;
                continue;
            }

            GLuint vbo;
            glGenBuffers(1, &vbo);
            vbos[i] = vbo;
            glBindBuffer(bufferView.target, vbo);
            glBufferData(bufferView.target, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset,
                         GL_STATIC_DRAW);
        }
        bool textureGenerated = false;
        const auto &mesh = model.meshes[meshIndex];
        for (const auto &primitive: mesh.primitives) {
            const auto &indexAccessor = model.accessors[primitive.indices];
            for (const auto &attrib: primitive.attributes) {
                tinygltf::Accessor accessor = model.accessors[attrib.second];
                int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

                int size = 1;
                if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                    size = accessor.type;
                }

                int vaa = -1;
                if (attrib.first.compare("POSITION") == 0) vaa = 0;
                if (attrib.first.compare("NORMAL") == 0) vaa = 1;
                if (attrib.first.compare("TEXCOORD_0") == 0) vaa = 2;
                if (vaa > -1) {
                    glEnableVertexAttribArray(vaa);
                    glVertexAttribPointer(vaa, size, accessor.componentType,
                                          accessor.normalized ? GL_TRUE : GL_FALSE,
                                          byteStride, BUFFER_OFFSET(accessor.byteOffset));
                } else
                    std::cout << "vaa missing: " << attrib.first << std::endl;

                /*
                 The tinygltf mesh primitives have 3 attributes: NORMAL, POSITION and TEXCOORD_0
                 for now we are only interested in first and the second, the third is just for textures we don't cover them at this point ^^
                 for more information see https://learnopengl.com/Getting-started/Textures
                */

                if (model.textures.size() > 0 && !textureGenerated) {
                    textureGenerated = true;
                    // fixme: Use material's baseColor
                    tinygltf::Texture &tex = model.textures[0];

                    if (tex.source > -1) {

                        tinygltf::Image &image = model.images[tex.source];
                        auto textureName = model.images[0].name;
                        //Loops through all textures and sees if it already is loaded. If it is, it returns the correct textureID
                        texture = GetTextureID(textureName);
                        //This checks if the Texture has already been loaded. If texture is 0 it hasn't been loaded yet
                        if (texture == 0) {
                            glGenTextures(1, &texture);
                            loadedTextures.push_back({textureName, texture});
                            glBindTexture(GL_TEXTURE_2D, texture);
                            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                            GLenum format = GL_RGBA;

                            if (image.component == 1) {
                                format = GL_RED;
                            } else if (image.component == 2) {
                                format = GL_RG;
                            } else if (image.component == 3) {
                                format = GL_RGB;
                            }
                            GLenum type = GL_UNSIGNED_BYTE;
                            if (image.bits == 16) {
                                type = GL_UNSIGNED_SHORT;
                            }
                            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                                         format, type, &image.image.at(0));

                        }
                    }
                }
            }
            primitives.push_back({primitive.mode, indexAccessor.count,
                                  indexAccessor.componentType});
        }
        if (Mesh::instancingAmount != 1){
            unsigned int buffer;
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, Mesh::instancingAmount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
            glBindVertexArray(VAO);
            // vertex attributes
            std::size_t vec4Size = sizeof(glm::vec4);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);
        }
        glBindVertexArray(0);
        for (auto &entry: vbos) {
            glDeleteBuffers(1, &entry.second);
            entry.second = 0;
        }
    }

    tinygltf::Model Mesh::loadGltf(const std::filesystem::path &gltfAssetPath) {
        tinygltf::TinyGLTF loader;
        tinygltf::Model model;
        std::string err;
        std::string warn;


        bool res;
        if (gltfAssetPath.extension().string() == ".glb") {
            res = loader.LoadBinaryFromFile(&model, &err, &warn, gltfAssetPath.string());
        } else {
            res = loader.LoadASCIIFromFile(&model, &err, &warn, gltfAssetPath.string());
        }

        if (!warn.empty()) {
            std::cerr << "[tinygltf] warning: " << warn << std::endl;
        }

        if (!err.empty()) {
            std::cerr << "[tinygltf] error: " << err << std::endl;
        }

        if (!res) {
            throw std::runtime_error("[tinyglt] failed to load glTF: " + gltfAssetPath.string());
        }

        return std::move(model);
    }
    int Mesh::GetTextureID(const std::string &textureName) {
        {
            for (auto &textureInfo: loadedTextures) {
                if (textureInfo.name == textureName) {
                    return textureInfo.texID;
                }
            }
            return 0;
        }
    }

    void Mesh::draw() const {
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, texture);
        for (auto &primitive: primitives) {
            if (Mesh::instancingAmount == 1) {
                glDrawElements(primitive.mode, primitive.count, primitive.type, nullptr);
            }else{
                glDrawElementsInstanced(primitive.mode, primitive.count, primitive.type, 0, Mesh::instancingAmount);
            }
        }
        glBindVertexArray(0);
    }



    Mesh::~Mesh() {
        for (auto &entry: buffers) {
            glDeleteBuffers(1, &entry.second);
        }
        glDeleteVertexArrays(1, &VAO);
    }

}