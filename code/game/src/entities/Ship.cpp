#include "Ship.h"
#include "../Game.h"

namespace gl3 {


    Ship::Ship(glm::vec3 position, float zRotation, glm::vec3 scale) : Entity(
            Shader("shaders/shaded/vertexShader.vert", "shaders/shaded/fragmentShader.frag"),
            Mesh("gltf/spaceship1.glb"),
            position,
            zRotation,
            scale,
            {0.4, 0.3, 1, 1}) {
    }

    void Ship::update(Game* game, float deltaTime) {
        auto window = game->getWindow();
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            zRotation += rotationSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            zRotation -= rotationSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            position.y += sin(glm::radians(zRotation)) * translationSpeed * deltaTime;
            position.x += cos(glm::radians(zRotation)) * translationSpeed * deltaTime;
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            position.y -= sin(glm::radians(zRotation)) * translationSpeed * deltaTime;
            position.x -= cos(glm::radians(zRotation)) * translationSpeed * deltaTime;
        }
        countdownUntilNextShot -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && countdownUntilNextShot <= 0)  {
            auto missile = std::make_unique<Missile>(game);
            missile->setPosition(position);
            missile->setZRotation(zRotation - 90);
            missile->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
            missiles.push_back(std::move(missile));
            countdownUntilNextShot = timeBetweenShots;
        }
        for (auto &m: missiles) {
            m->update(game, deltaTime);
        }
        if (missiles.size() >= 100) {
            missiles.erase(missiles.begin());
        }
    }
    void Ship::draw(Game *game) {
        Entity::draw(game);
        for (auto &m: missiles) {
            m->draw(game);
        }
    }
}