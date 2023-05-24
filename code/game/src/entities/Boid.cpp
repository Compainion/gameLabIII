//
// Created by Fabian on 16/05/2023.
//

#include <random>
#include "Boid.h"

namespace gl3 {
    Boid::Boid(glm::vec3 position) : Entity(Shader("shaders/shaded/vertexShader.vert", "shaders/shaded/fragmentShader.frag"),
                          Mesh("gltf/boid.glb"), {5.0f,5.0f,5.0f,}, 0.0f, {0.1,0.1,0.1} ){
    this->position = position;
    setRandomStartingVelocity();
    }

    void Boid::setRandomStartingVelocity(){
        std::mt19937 randomNumberEngine{static_cast<unsigned int>(glfwGetTime() * 100)};
        std::uniform_real_distribution distribution(-0.05, 0.05);
        velocity = {distribution(randomNumberEngine), distribution(randomNumberEngine), distribution(randomNumberEngine)};
    }


    void Boid::update(gl3::Game *game, float deltaTime) {
        Entity::update(game, deltaTime);
        position += velocity;
        acceleration = limit(acceleration, maxForce);
        velocity += acceleration * deltaTime;
        velocity = limit(velocity, maxSpeed);
    }

    glm::vec3 Boid::align(const std::vector<Boid*> &boids) {
        glm::vec3 steering = {0.0f, 0.0f, 0.0f};
        float distance;
        int size = 0;
        assert(!boids.empty());
        for (auto& other : boids) {
            distance = glm::length(position - other->position);
            if (this != other && distance < perception) {
                steering += other->velocity;
                size++;
            }
        }
        //calculate the average velocity vector
        if(size > 0) {
            steering = steering / static_cast<float>(size);
            steering -= velocity;
            steering = glm::normalize(steering);
        }
        return steering;
    }

    glm::vec3 Boid::cohere(const std::vector<Boid*> &boids){
        glm::vec3 steering = {0.0f, 0.0f, 0.0f};
        float distance;
        int size = 0;
        for (auto& other : boids) {
            distance = glm::length(position - other->position);
            if (this != other && distance < perception) {
                steering += other->position;
                size++;
            }
        }
        if (size > 0) {
            steering = steering / static_cast<float>(size);
            steering -= position;
            steering = glm::normalize(steering);
        }
        return steering;
    }

    glm::vec3 Boid::separate(const std::vector<Boid *> &boids) {
        glm::vec3 steering = {0.0f, 0.0f, 0.0f};
        float distance;
        int size =0;
        assert(!boids.empty());
        for (auto& other : boids) {
            distance = glm::length(position - other->position);
            if (this != other && distance < perception) {
                glm::vec3 difference = position - other->position;
                difference = (difference / distance) / distance;
                steering += difference;
                size++;
            }
        }
        //calculate the average velocity vector
        if (size > 0 ) {
            steering = steering / static_cast<float>(size);
            steering -= velocity;
            steering = glm::normalize(steering);
        }
        return steering;
    }



    void Boid::flock(const std::vector<Boid*> &boids){
        acceleration = {0.0f, 0.0f, 0.0f};
        glm::vec3 alignment = align(boids);
        glm::vec3 cohesion = cohere(boids);
        glm::vec3 separation = separate(boids);
        glm::vec3 targetPosition = glm::vec3 {100.0f, 0, 0};
        //glm::vec3 target = calculateTargetVector(targetPosition);
        acceleration += alignment * alignmentMagnitude;
        acceleration += cohesion * cohesionWeight;
        acceleration += separation * separationWeight;
        //acceleration += target * targetWeight;
    }


    //utility Function for Limiting a Vector to a maximum Magnitude
    glm::vec3 Boid::limit(glm::vec3 vector, float max){
        if (glm::length(vector) > max)
            vector = glm::normalize(vector) * max;
        return vector;
    }
    glm::vec3 Boid::setMagnitude(glm::vec3 vector, float magnitude){
        return glm::normalize(vector) * magnitude;
    }

    glm::vec3 Boid::calculateTargetVector(glm::vec3 targetPosition) {
        return glm::normalize(targetPosition - position);
    }
}