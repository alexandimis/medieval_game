# Cmake project config
cmake_minimum_required(VERSION 4.2)
project(medieval_game C)
set(CMAKE_C_STANDARD 11)

# SDL3 options config
set(SDL_SHARED ON CACHE BOOL "Build shared library")
set(SDL_STATIC OFF CACHE BOOL "Build static library")
set(SDL_TEST OFF CACHE BOOL "Disable SDL tests")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the SDL3 library
add_subdirectory(deps/SDL)
add_subdirectory(deps/SDL_image)

# Game executable
add_executable(game
        source/main.c
        source/main.h
        source/renderer.c
        source/renderer.h
        source/player.c
        source/player.h
        source/terrain.c
        source/terrain.h
        source/fileio.c
        source/fileio.h
        source/game.c
        source/macros.h
)

# Include external header files
target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/deps/uthash
)

# Link SDL3 to the game
target_link_libraries(game PRIVATE
        SDL3::SDL3-shared
        SDL3_image::SDL3_image-shared
)
