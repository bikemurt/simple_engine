cmake_minimum_required(VERSION 3.16)
project(simple_engine LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Stb REQUIRED)

find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")

set(PRJ_HEADERS

    )

set(PRJ_SOURCES
    src/main.cpp
    src/core/model_loader.cpp
    src/core/renderer.cpp
    )

add_executable(${PROJECT_NAME} ${PRJ_HEADERS} ${PRJ_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${TINYGLTF_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
