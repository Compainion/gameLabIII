//
// Created by Fabian on 17/05/2023.
//

#pragma once
#include <vector>
#include "Boid.h"
namespace gl3 {
    class Swarm : public Entity{
    public:
        explicit Swarm();
        std::vector<std::unique_ptr<Boid>> boids;
        void update(gl3::Game *game, float deltaTime) override;
    };

}