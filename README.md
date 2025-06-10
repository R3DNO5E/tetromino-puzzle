# Tetris C++ SDL Implementation

A C++ port of the Python Tetris game using SDL2 for graphics and input handling.

## Features

- Classic Tetris gameplay with all 7 tetromino pieces (I, O, T, S, Z, J, L)
- Real-time piece rotation and movement
- Line clearing with scoring system
- Progressive difficulty levels
- Auto-play AI mode
- SDL2-based GUI with colorful graphics

## Controls

- **A/D** or **←/→**: Move piece left/right
- **S** or **↓**: Soft drop (move piece down faster)
- **W** or **↑**: Rotate piece
- **Space**: Hard drop (instantly drop piece to bottom)
- **T**: Toggle auto-play mode
- **Q**: Quit game

## Building

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.16 or later
- SDL2 development libraries

### Ubuntu/Debian

```bash
# Option 1: Install dependencies manually
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libsdl2-ttf-dev pkg-config

# Option 2: Use provided script
sudo ./install_deps.sh
```

### Build Instructions

```bash
cd tetris
mkdir build
cd build
cmake ..
make
```

**Note**: If you don't have sudo access, you may need to install SDL2 through other means or ask your system administrator to install the development libraries.

### Running

```bash
./tetris
```

## Architecture

The implementation consists of three main components:

- **TetrisGame**: Core game logic, piece management, collision detection, scoring
- **Renderer**: SDL2-based graphics rendering, UI display
- **main**: Game loop, input handling, timing

## Game Features

- **Scoring**: Points awarded for line clears, multiplied by current level
- **Levels**: Difficulty increases every 10 lines cleared
- **Auto-play**: AI mode using board evaluation heuristics
- **Collision Detection**: Accurate piece placement and boundary checking
- **Line Clearing**: Full horizontal lines are cleared and board compacts

The auto-play AI evaluates board positions using multiple factors:
- Aggregate height penalty
- Complete lines bonus  
- Holes penalty
- Bumpiness (height variation) penalty

## Differences from Python Version

- Uses SDL2 instead of curses for cross-platform GUI
- Improved graphics with colored pieces
- Better input handling with both WASD and arrow keys
- More robust timing system using high-resolution clock