//
// Created by Megaport on 04/04/2023.
//

#include "Light.h"
#include "../Game.h"

namespace gl3{
    Light::Light() : Entity(Shader("shaders/shaded/vertexShader.vert", "shaders/lightSource/lightSourceShader.frag"),
                            Mesh("gltf/planet1.glb"))
    {}

    void Light::update(Game *game, float deltaTime) {
        Entity::update(game, deltaTime);
        float posX = sin(glfwGetTime() + 30) * 0.8;
        float posZ = sin(glfwGetTime()) * 0.8;
        position = glm::vec3{posX, 10, posZ};
        game->lightPosition = position;
    }
}
