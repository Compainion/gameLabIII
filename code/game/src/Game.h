#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "rendering/Shader.h"
#include "rendering/Mesh.h"
#include "entities/Ship.h"
#include <memory>
#include "entities/Planet.h"
#include "Camera.h"
#include "entities/Boid.h"
#include "reactphysics3d/reactphysics3d.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace r3d=reactphysics3d;

namespace gl3 {
    class Game {
    public:
        const static unsigned int width = 1920;
        const static unsigned int height = 1080;
        glm::vec3 lightPosition = {0.0f,0.0f,0.0f};
        glm::vec3 cameraPosition = {0.0f,2.0f,0.0f};
        glm::mat4 calculateMvpMatrix(glm::vec3 position, float zRotationInDegrees, glm::vec3 scale);
        Ship *getShip() { return ship; }
        GLFWwindow *getWindow(){return window;}
        r3d::PhysicsCommon &getPhysicsCommon(){return physicsCommon;}
        r3d::PhysicsWorld &getPhysicsWorld(){return *physicsWorld;}

        Game(const std::string &title);

        void run();

        virtual ~Game();

    private:
        void gui();
        void update();
        void draw();
        void physics();
        void updateDeltaTime();
        void processCameraInput();

        std::vector<Boid*> swarm1;

        std::vector<std::unique_ptr<Entity>> entities;
        GLFWwindow *window = nullptr;
        Ship *ship = nullptr;
        float lastFrameTime = 1.0f/60;
        float deltaTime = 1.0f/60;
        const float timeStep = 1.0f/60;
        r3d::PhysicsCommon physicsCommon;
        r3d::PhysicsWorld *physicsWorld = nullptr;
    };
}

