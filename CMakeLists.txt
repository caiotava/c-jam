cmake_minimum_required(VERSION 3.21)
project(c_jam C)

add_compile_definitions(CUTE_TILED_IMPLEMENTATION)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

include_directories(
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
)

set(CMAKE_C_STANDARD 99)

add_executable(c_jam main.c game/game.c render/image.c game/scene.c game/player.c)

target_include_directories(c_jam PRIVATE ${PROJECT_SOURCE_DIR})

# Set the link libraries
target_link_libraries(
        ${PROJECT_NAME}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
)

file(COPY assets DESTINATION .)
file(COPY assets/maps/tilesets DESTINATION ./assets)