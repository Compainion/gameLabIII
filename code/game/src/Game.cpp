#include "Game.h"
#include "random"
#include "entities/Enemy.h"
#include "Camera.h"

namespace gl3 {
    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    Camera camera((glm::vec3(0.0f,2.0f,0.0f)));
    float lastX = Game::width / 2.0f;
    float lastY = Game::height / 2.0f;
    bool firstMouse = true;

    glm::mat4 Game::calculateMvpMatrix(glm::vec3 position, float zRotationInDegrees, glm::vec3 scale) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        model = glm::rotate(model, glm::radians(zRotationInDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 100.0f);
        return projection * view * model;
    };

    Game::Game(const std::string &title) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize glfw");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (window == nullptr) {
            throw std::runtime_error("Failed to create window");
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glEnable(GL_DEPTH_TEST);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("gl error");
        }
    }

    void Game::run() {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        std::mt19937 randomNumberEngine{};
        std::uniform_real_distribution positionDist{-2.0, 2.0};
        std::uniform_real_distribution scaleDist{0.01, 0.2};
        std::uniform_real_distribution colorDistR{0.0, 1.0};
        std::uniform_real_distribution colorDistG{0.0, 1.0};
        std::uniform_real_distribution colorDistB{0.0, 1.0};
        for (auto i = 0; i < 1; ++i) {
            auto randomPosition = glm::vec3(static_cast<float>(positionDist(randomNumberEngine) * 1.5),
                                            static_cast<float>(positionDist(randomNumberEngine) * 1.5), -1);
            auto scale = static_cast<float>(scaleDist(randomNumberEngine));
            auto randomScale = glm::vec3(scale, scale, scale);
            auto r = colorDistR(randomNumberEngine);
            auto g = colorDistG(randomNumberEngine);
            auto b = colorDistB(randomNumberEngine);
            auto randomColor = glm::vec4(r, g , b, 1.0f);
            auto planet = std::make_unique<Planet>(randomPosition, scale, randomColor);
            entities.push_back(std::move(planet));
        }

        auto enemy = std::make_unique<Enemy>(glm::vec3(2, 0, 0), -90, 0.25f);
        entities.push_back(std::move(enemy));


        auto spaceShip = std::make_unique<Ship>(glm::vec3(-2, 1, 0));
        ship = spaceShip.get();
        entities.push_back(std::move(spaceShip));


        glfwSetTime(1.0 / 60);
        while (!glfwWindowShouldClose(window)) {
            update();
            draw();
            updateDeltaTime();
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
    }

    void Game::update() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        for (auto &entity: entities) {
            entity->update(this, deltaTime);
        }
        processCameraInput();
    }

    void Game::draw() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto &entity: entities) {
            entity->draw(this);
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
    }

    void Game::updateDeltaTime() {
        auto frameTime = (float) glfwGetTime();
        deltaTime = frameTime - lastFrameTime;
        lastFrameTime = frameTime;
    }

    void Game::processCameraInput() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            camera.ProcessKeyboard(UPWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            camera.ProcessKeyboard(DOWNWARD, deltaTime);
    }

    void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
    {
        float xpos = static_cast<float>(xPosIn);
        float ypos = static_cast<float>(yPosIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset, true);
    }


    Game::~Game() {
        glfwTerminate();
    }


}