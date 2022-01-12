cmake_minimum_required(VERSION 3.21)
project(c_jam C)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

include_directories(
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
)

set(CMAKE_C_STANDARD 99)

add_executable(c_jam main.c game/game.c)

# Set the link libraries
target_link_libraries(
        ${PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
)