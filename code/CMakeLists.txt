cmake_minimum_required(VERSION 3.23)
project(GameLabIII)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(game)

add_subdirectory(extern/glad)

add_subdirectory(extern/glfw)

add_subdirectory(extern/glm)

add_subdirectory(extern/tinygltf)

