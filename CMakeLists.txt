cmake_minimum_required(VERSION 3.16)
project(BlockDrop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# Create executable
add_executable(blockdrop
    src/main.cpp
    src/BlockDropGame.cpp
    src/Renderer.cpp
)

# Include directories
target_include_directories(blockdrop PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(blockdrop ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})
target_compile_options(blockdrop PRIVATE ${SDL2_CFLAGS_OTHER} ${SDL2_TTF_CFLAGS_OTHER})

# Compiler flags
target_compile_options(blockdrop PRIVATE -Wall -Wextra)