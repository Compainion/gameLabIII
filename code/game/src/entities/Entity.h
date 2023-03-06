//
// Created by Megaport on 06/02/2023.
//

#pragma once

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "../rendering/Shader.h"
#include "../rendering/Mesh.h"
#include "GLFW/glfw3.h"


namespace gl3 {
    class Game;

    class Entity {
    protected:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec4 color;
        float zRotation;

    private:
        Shader shader;
        Mesh mesh;

        [[nodiscard]] glm::mat4 calculateModelToWorldNormal() const;


    public:
        Entity(Shader shader,
               Mesh mesh,
               glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
               float zRotation = 0.0f,
               glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
               glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        virtual void update(Game *game, float deltaTime) {}

        virtual void draw(Game *game);

        virtual ~Entity() = default;

        [[nodiscard]] const auto &getBody() const { return position; }

        [[nodiscard]] const glm::vec3 &getPosition() const { return position; }

        [[nodiscard]] float getZRotation() const { return zRotation; }

        [[nodiscard]] const glm::vec3 &getScale() const { return scale; }

        void setPosition(const glm::vec3 &position) { Entity::position = position; }

        void setZRotation(float zRotation) { Entity::zRotation = zRotation; }

        void setScale(const glm::vec3 &scale) { Entity::scale = scale; }

    };
}