#pragma once
#include "Entity.h"

namespace gl3 {
    class Boid : public Entity{
    public:
        explicit Boid(glm::vec3 position);
        void flock(const std::vector<Boid*> &boids);
        float alignmentMagnitude {2.0f}; //2.0
        float cohesionWeight {5.0f}; //5.0
        float separationWeight {0.2f}; //0.2
        float targetWeight {5.0};
        float randomWeight {1.0};
    private:
        glm::vec3 velocity {0.0f, 0.0f, 0.0f};
        glm::vec3 acceleration {0.0f, 0.0f, 0.0f};
        glm::vec3 cameraPosition;
        float perception = 5.5f; //5.5
        float maxForce = 0.05; //0.02
        float maxSpeed = 0.05; //0.04
        void update(gl3::Game *game, float deltaTime) override;
        glm::vec3 align(const std::vector<Boid*> &boids);
        glm::vec3 cohere(const std::vector<Boid*> &boids);
        glm::vec3 separate(const std::vector<Boid*> &boids);
        glm::vec3 calculateTargetVector(glm::vec3 targetPosition);
        static glm::vec3 limit(glm::vec3 vector, float max);
        static glm::vec3 setMagnitude(glm::vec3 vector, float magnitude);
        void setRandomStartingVelocity();
    };

} // gl3

