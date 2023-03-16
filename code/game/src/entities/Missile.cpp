#include "Missile.h"
#include "Entity.h"

namespace gl3 {

    Missile::Missile(gl3::Game *game) : Entity(
            Shader("shaders/shaded/vertexShader.vert", "shaders/shaded/fragmentShader.frag"),
            Mesh("gltf/missiles1.glb"),
            {0.0f, 0.0f, 0.0f},
            0.0f,
            {1, 1, 1},
            {1.0f, 1.0f, 1.0f, 1.0f}) {}

    void Missile::update(gl3::Game *game, float deltaTime) {
        position.y = getPosition().y - sin(glm::radians(zRotation - 90.0f)) * speed * deltaTime;
        position.x = getPosition().x - cos(glm::radians(zRotation - 90.0f)) * speed * deltaTime;
    }
}