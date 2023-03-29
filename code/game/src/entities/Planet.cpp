//
// Created by Megaport on 07/02/2023.
//

#include "Planet.h"

namespace gl3 {

    Planet::Planet(glm::mat4 modelMatrices[], int amount) : Entity(
            Shader("shaders/instanceShader.vert", "shaders/fragmentShader.frag"),
            Mesh("gltf/planet1.glb", 0 , amount , modelMatrices)) {}

    void Planet::draw(Game *game) {
        Entity::draw(game);
        shader.setFloat("time",sin(glfwGetTime()));
    }
}// gl3