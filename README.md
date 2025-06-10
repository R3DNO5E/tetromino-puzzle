# BlockDrop

A C++ implementation of a falling blocks puzzle game with an advanced AI auto-play mode, built using SDL2 for cross-platform graphics.

## Features

- **Classic Falling Blocks Gameplay**: All 7 standard tetromino pieces (I, O, T, S, Z, J, L)
- **Intelligent AI**: Advanced auto-play mode with safety-prioritized decision making
- **Modern Graphics**: SDL2-based rendering with colorful piece visualization
- **Progressive Difficulty**: Automatic level progression and speed increases
- **Responsive Controls**: Support for both WASD and arrow key inputs

## Controls

| Key | Action |
|-----|--------|
| **A/D** or **←/→** | Move piece left/right |
| **S** or **↓** | Soft drop (accelerated descent) |
| **W** or **↑** | Rotate piece clockwise |
| **Space** | Hard drop (instant placement) |
| **T** | Toggle auto-play mode |
| **Q** | Quit game |

## Building

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.16 or later
- SDL2 development libraries

### Installation (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libsdl2-ttf-dev pkg-config

# Or use the provided script
sudo ./install_deps.sh
```

### Build and Run

```bash
mkdir build
cd build
cmake ..
make
./blockdrop
```

## AI Algorithm

The auto-play mode features a sophisticated evaluation function that prioritizes:

1. **Game Over Prevention**: Extreme penalties for dangerous board heights
2. **Line Clearing**: Rewards for completing horizontal lines
3. **Board Safety**: Penalties for holes, uneven surfaces, and excessive height
4. **Strategic Positioning**: Controlled piece placement with optimized timing

The AI ensures piece rotation and horizontal movement complete before allowing piece descent, preventing premature drops that could lead to suboptimal placements.

## Architecture

- **BlockDropGame** (`src/BlockDropGame.cpp`): Core game logic, AI evaluation, and piece management
- **Renderer** (`src/Renderer.cpp`): SDL2-based graphics and visual rendering
- **main** (`src/main.cpp`): Game loop, input handling, and timing control

## Project Structure

```
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── install_deps.sh         # Dependency installation script
├── include/                # Header files
│   ├── Renderer.h
│   └── BlockDropGame.h
└── src/                    # Source files
    ├── Renderer.cpp
    ├── BlockDropGame.cpp
    └── main.cpp
```

## License

This project is licensed under the GNU General Public License v3.0 (GPLv3).

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.