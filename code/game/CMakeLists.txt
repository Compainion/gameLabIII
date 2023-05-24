cmake_minimum_required(VERSION 3.18)

# set variables for source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/**.cpp" "src/**.h")

# set executable name
set(EXE_FILE Game)

# set the imgui path variable
set(IMGUI_PATH "../extern/imgui")

#include the needed imgui files as IMGUI_SOURCES
include_directories(${IMGUI_PATH})
file(GLOB IMGUI_SOURCES ${IMGUI_PATH}/*.cpp ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)

# add the executable target
add_executable(${EXE_FILE} ${SOURCE_FILES} ${IMGUI_SOURCES})

# require C++ 17 compiler
target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)

target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm tinygltf reactphysics3d)

target_compile_definitions(${EXE_FILE} PRIVATE DEBUG_ASSET_ROOT=${PROJECT_SOURCE_DIR}/assets)
