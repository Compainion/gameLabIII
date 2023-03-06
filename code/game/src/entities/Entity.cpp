#include "Entity.h"
#include "../Game.h"

namespace gl3 {
    Entity::Entity(Shader shader, Mesh mesh, glm::vec3 position, float zRotation, glm::vec3 scale, glm::vec4 color) :
            shader(std::move(shader)),
            mesh(std::move(mesh)),
            position(position),
            zRotation(zRotation),
            scale(scale),
            color(color) {}


    glm::mat4 Entity::calculateModelToWorldNormal() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        model = glm::rotate(model, glm::radians(zRotation), glm::vec3(0.0f, 0.0f, 1.0f));
        return glm::transpose(glm::inverse(model));
    }



    void Entity::draw(Game *game) {
        auto mvpMatrix = game->calculateMvpMatrix(position, zRotation, scale);
        shader.use();
        shader.setMatrix("mvp", mvpMatrix);
        shader.setMatrix("modelToWorldNormal", calculateModelToWorldNormal());
        shader.setVector("color", color);
        mesh.draw();
    }
}
