cmake_minimum_required(VERSION 3.18)

# set variables for source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/**.cpp" "src/**.h")

# set executable name
set(EXE_FILE Game)

# add the executable target
add_executable(${EXE_FILE} ${SOURCE_FILES})

# require C++ 17 compiler
target_compile_features(${EXE_FILE} PRIVATE cxx_std_17)

target_link_libraries(${EXE_FILE} PRIVATE glad glfw glm tinygltf reactphysics3d)

target_compile_definitions(${EXE_FILE} PRIVATE DEBUG_ASSET_ROOT=${PROJECT_SOURCE_DIR}/assets)
