#include "Game.h"
#include "random"
#include "entities/Enemy.h"
#include "Camera.h"
#include "entities/Light.h"
#include "entities/Terrain.h"
#include "entities/Swarm.h"

namespace gl3 {
    void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    void mouseSwitchCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetKeyCallback(window, mouseSwitchCallback);
        glEnable(GL_DEPTH_TEST);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error("gl error");
        }
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 460");
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
        int amount = 200000;
        auto *modelMatrices = new glm::mat4[amount];
        for (auto i = 0; i < amount; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 randomPosition = glm::vec3(static_cast<float>(positionDist(randomNumberEngine) * 35),
                                            static_cast<float>((positionDist(randomNumberEngine) + 2.5) * 25),
                                            static_cast<float>(positionDist(randomNumberEngine) * 35));
            model = glm::translate(model, randomPosition);
            auto scale = static_cast<float>(scaleDist(randomNumberEngine));
            auto randomScale = glm::vec3(scale, scale, scale);
            model = glm::scale(model, randomScale);
            modelMatrices[i] = model;
        }
        auto planet = std::make_unique<Planet>(modelMatrices, amount);
        entities.push_back(std::move(planet));

        auto enemy = std::make_unique<Enemy>(glm::vec3(2, 0, 0), -90, 0.25f);
        entities.push_back(std::move(enemy));


        auto light = std::make_unique<Light>();
        entities.push_back(std::move(light));

        auto spaceShip = std::make_unique<Ship>(glm::vec3(-2, 1, 0));
        ship = spaceShip.get();
        entities.push_back(std::move(spaceShip));

        //auto terrain = std::make_unique<Terrain>(20.0f);
        //entities.push_back(std::move(terrain));



        for (int i = 0; i < 50; ++i) {
            glm::vec3 randomPosition = {static_cast<float>(positionDist(randomNumberEngine)), static_cast<float>(positionDist(randomNumberEngine)),static_cast<float>(positionDist(randomNumberEngine))};
            auto boid = std::make_unique<Boid>(randomPosition);
            swarm1.push_back(boid.get());
            entities.push_back(std::move(boid));
        }

        //initialize Physics world
        physicsWorld = physicsCommon.createPhysicsWorld();

        bool show_demo_window = true;
        glfwSetTime(1.0 / 60);
        while (!glfwWindowShouldClose(window)) {
            gui();
            update();
            draw();
            physics();
            updateDeltaTime();
            glfwPollEvents();
        }
        glDeleteVertexArrays(1, &VAO);
    }

    void Game::gui(){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::SliderFloat("alignmentWeight", &Boid::alignmentWeight, 0.0f, 100.0f);
        ImGui::SliderFloat("cohesionWeight", &Boid::cohesionWeight, 0.0f, 100.0f);
        ImGui::SliderFloat("separationWeight", &Boid::separationWeight, 0.0f, 100.0f);
        ImGui::SliderFloat("separationWeight", &Boid::perception, 0.0f, 100.0f)
        // Edit 1 float using a slider from 0.0f to 1.0f
        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)");
        ImGui::End();
        ImGui::Render();
    }

    void Game::update() {
        for (auto &entity: entities) {
            entity->update(this, deltaTime);
        }
        for (auto &boid: swarm1) {
            boid->flock(swarm1);
        }
        cameraPosition = camera.Position;
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
        //This is here so the GUI gets drawn in front of the entities
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    void Game::physics() {
        physicsWorld->update(timeStep);
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
        if (!camera.cameraMouseMovement) return;
        camera.ProcessMouseMovement(xoffset, yoffset, true);
    }

    Game::~Game() {
        glfwTerminate();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }


    void mouseSwitchCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
            if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
                camera.cameraMouseMovement = false;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                camera.cameraMouseMovement = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
}