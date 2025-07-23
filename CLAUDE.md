# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Status

This is a C++ minesweeper implementation with SDL2 graphics and mouse control. The project includes a complete working game with mine placement, cell revelation, flagging, and win/lose conditions in a native GUI window.

## Prerequisites

### macOS
```bash
brew install sdl2 sdl2_ttf
```

### Ubuntu/Debian
```bash
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```

### Arch Linux
```bash
sudo pacman -S sdl2 sdl2_ttf
```

## Development Commands

### Building and Running
- `make` - Build the game (creates `minesweeper` executable)
- `make run` - Build and run the game
- `make clean` - Clean build artifacts
- `make debug` - Build with debug symbols and DEBUG flag
- `make release` - Build optimized release version
- `./minesweeper` - Run the compiled game directly

### Development Tools
- `valgrind ./minesweeper` - Run with memory checking (requires valgrind installation)
- `gdb ./minesweeper` - Debug with GDB

## Project Structure

```
minesweeper/
├── src/                    # Source files
│   ├── main.cpp           # Main game loop and SDL initialization
│   ├── Minesweeper.cpp    # Game logic implementation
│   └── GameRenderer.cpp   # SDL2 rendering and graphics
├── include/               # Header files
│   ├── Minesweeper.h      # Game logic interface
│   └── GameRenderer.h     # Rendering interface
├── build/                 # Build artifacts (generated)
├── Makefile              # Build configuration with SDL2 linking
└── CLAUDE.md             # This file
```

## Game Features

- 9x9 grid with 10 mines (standard beginner configuration)
- SDL2-based GUI with mouse control
- Left-click to reveal cells, right-click to flag
- Cell revelation with automatic flood-fill for empty areas
- Visual feedback with colors and numbers
- Win/lose detection with status display
- Press 'R' to restart the game
- Random mine placement

## Current Implementation Details

### Core Classes
- `Minesweeper` class handles all game logic in `src/Minesweeper.cpp`
- `GameRenderer` class manages SDL2 graphics and input in `src/GameRenderer.cpp`
- Uses `std::vector<std::vector<int>>` for the game board
- Separate boolean matrices track revealed and flagged states

### Key Methods
- `Minesweeper::initializeBoard()` - Random mine placement and number calculation
- `Minesweeper::reveal(x, y)` - Cell revelation with recursive flood-fill
- `Minesweeper::toggleFlag(x, y)` - Flag/unflag cells
- `GameRenderer::render()` - SDL2 rendering of game state
- `GameRenderer::getCellFromMouse()` - Mouse coordinate to grid conversion

### Game Controls
- Left mouse click - Reveal cell
- Right mouse click - Toggle flag
- 'R' key - Restart game
- Close window or Alt+F4 - Quit

### Technical Notes
- C++17 standard required
- SDL2 and SDL2_ttf dependencies
- Uses `<random>` for mine placement
- Memory-safe with RAII and STL containers
- Attempts to load system fonts (Arial on macOS, DejaVu on Linux)
- 60 FPS rendering with SDL_Delay(16)