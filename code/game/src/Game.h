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


namespace gl3 {
    class Game {
    public:
        const static unsigned int width = 1280;
        const static unsigned int height = 720;
        glm::vec3 lightPosition = {0.0f,0.0f,0.0f};
        glm::vec3 cameraPosition = {0.0f,2.0f,0.0f};
        Game(const std::string &title);
        void run();
        glm::mat4 calculateMvpMatrix(glm::vec3 position, float zRotationInDegrees, glm::vec3 scale);
        Ship *getShip() { return ship; }
        GLFWwindow *getWindow(){return window;}
        virtual ~Game();

    private:
        void update();
        void draw();
        void updateDeltaTime();
        void processCameraInput();


        GLFWwindow *window = nullptr;
        Ship *ship = nullptr;
        std::vector<std::unique_ptr<Entity>> entities;
        float lastFrameTime = 1.0f/60;
        float deltaTime = 1.0f/60;
    };
}

