cmake_minimum_required(VERSION 3.16)
project(Tetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# Create executable
add_executable(tetris
    src/main.cpp
    src/TetrisGame.cpp
    src/Renderer.cpp
)

# Include directories
target_include_directories(tetris PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(tetris ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
target_compile_options(tetris PRIVATE ${SDL2_CFLAGS_OTHER} ${SDL2_TTF_CFLAGS_OTHER})

# Compiler flags
target_compile_options(tetris PRIVATE -Wall -Wextra)