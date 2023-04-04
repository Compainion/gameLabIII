//
// Created by Megaport on 04/04/2023.
//

#include "Light.h"
#include "../Game.h"

namespace gl3{
    Light::Light(glm::vec3 position) : Entity(Shader("shaders/shaded/vertexShader.vert", "shaders/lightSource/lightSourceShader.frag"),
                            Mesh("gltf/planet1.glb"))
    {}

    void Light::update(Game *game, float deltaTime) {
        Entity::update(game, deltaTime);
        position = game->lightPosition;
    }
}
