//
// Created by Megaport on 08/02/2023.
//

#pragma once
#include "Entity.h"
#include "Missile.h"

namespace gl3 {

    class Enemy : public Entity{
    public:
        float speed = 0.5f;
        explicit Enemy(glm::vec3 position, float zRotation, float size);
        void update(Game *game, float deltaTime) override;
        void draw(Game *game) override;
    private:
        std::vector<std::unique_ptr<Missile>> missiles;
        const float timeBetweenShots = 0.1;
        float countdownUntilNextShot  {timeBetweenShots};
    };

} // gl3
