#include <iostream>
#include <SDL.h>
#include "Minesweeper.h"
#include "GameRenderer.h"

int main() {
    const int GRID_WIDTH = 9;
    const int GRID_HEIGHT = 9;
    const int MINE_COUNT = 10;
    const int WINDOW_WIDTH = GRID_WIDTH * 32 + 40;
    const int WINDOW_HEIGHT = GRID_HEIGHT * 32 + 100;
    
    Minesweeper game(GRID_WIDTH, GRID_HEIGHT, MINE_COUNT);
    GameRenderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return -1;
    }
    
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                    
                case SDL_MOUSEBUTTONDOWN: {
                    int cellX, cellY;
                    renderer.getCellFromMouse(event.button.x, event.button.y, cellX, cellY, game);
                    
                    if (cellX >= 0 && cellY >= 0) {
                        if (event.button.button == SDL_BUTTON_LEFT) {
                            // Left click - reveal cell
                            if (!game.isGameOver() && !game.isGameWon()) {
                                game.reveal(cellX, cellY);
                            }
                        } else if (event.button.button == SDL_BUTTON_RIGHT) {
                            // Right click - toggle flag
                            if (!game.isGameOver() && !game.isGameWon()) {
                                game.toggleFlag(cellX, cellY);
                            }
                        }
                    }
                    break;
                }
                
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_r) {
                        // R key - restart game
                        game.reset();
                    }
                    break;
            }
        }
        
        renderer.render(game);
        SDL_Delay(16); // ~60 FPS
    }
    
    return 0;
}