cmake_minimum_required(VERSION 3.16)
project(simple_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt REQUIRED)
find_package(bgfx CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")

set(PRJ_HEADERS

    )

set(PRJ_SOURCES
    src/main.cpp
    src/core/renderer.cpp
    src/core/utils/fileops.cpp
    )

add_executable(${PROJECT_NAME} ${PRJ_HEADERS} ${PRJ_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${TINYGLTF_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE bgfx::bx bgfx::bgfx bgfx::bimg bgfx::bimg_decode)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)