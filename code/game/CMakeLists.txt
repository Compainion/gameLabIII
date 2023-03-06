cmake_minimum_required(VERSION 3.18)

# set variables for source files
set(SOURCE_FILES src/main.cpp src/Game.cpp src/Game.h src/rendering/Shader.cpp src/rendering/Shader.h src/rendering/Mesh.cpp src/rendering/Mesh.h src/entities/Entity.cpp src/entities/Entity.h src/entities/Ship.cpp src/entities/Ship.h src/Assets.h src/entities/Planet.cpp src/entities/Planet.h src/entities/Enemy.cpp src/entities/Enemy.h src/entities/Missile.cpp src/entities/Missile.h src/Camera.cpp src/Camera.h src/Camera.h)

# set executable name
set(EXE_FILE Game)

# add the executable target
add_executable(${EXE_FILE} ${SOURCE_FILES})

# require C++ 17 compiler
target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)

target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm tinygltf)

target_compile_definitions(${EXE_FILE} PRIVATE DEBUG_ASSET_ROOT=${PROJECT_SOURCE_DIR}/assets)
