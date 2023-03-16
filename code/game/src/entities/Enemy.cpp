//
// Created by Megaport on 08/02/2023.
//

#include "Enemy.h"
#include "../Game.h"

namespace gl3 {
    Enemy::Enemy(glm::vec3 position, float zRotation, float size) :
            Entity(Shader("shaders/shaded/vertexShader.vert",
                          "shaders/shaded/fragmentShader.frag"),
                   Mesh("gltf/enemy2.glb"),
                   position,
                   zRotation,
                   glm::vec3(size, size, size),
                   glm::vec4(0.35f, 0.35f, 0.35f, 1.0f)
            ) {
    }
    float lerp(float a, float b, float f) {
        return a + f * (b - a);
    }

    void Enemy::update(Game *game, float deltaTime) {
        std::time_t elapsedTime = std::time(nullptr);
        const auto shipPosition = game->getShip()->getPosition();
        auto distanceToShip = glm::distance(position, shipPosition);
        float delta_x = this->getPosition().x - shipPosition.x;
        float delta_y = this->getPosition().y - shipPosition.y;
        float theta_radians = atan2(delta_y, delta_x);

        zRotation = glm::degrees(theta_radians) - 90.0f;
        if (distanceToShip >= 0.5f) {
            position.x = lerp(position.x, shipPosition.x, deltaTime * speed);
            position.y = lerp(position.y, shipPosition.y, deltaTime * speed);
        }

        countdownUntilNextShot -= deltaTime;
        if (elapsedTime % 5 == 0 && countdownUntilNextShot <= 0) {
            auto missile = std::make_unique<Missile>(game);
            missile->setPosition(this->getPosition());
            missile->setZRotation(this->getZRotation() - 180);
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

    void Enemy::draw(Game *game) {
        Entity::draw(game);
        for (auto &m: missiles) {
            m->draw(game);
        }
    }
} // gl3