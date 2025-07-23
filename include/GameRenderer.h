#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "Minesweeper.h"

class GameRenderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    
    static const int CELL_SIZE = 32;
    static const int GRID_OFFSET_X = 20;
    static const int GRID_OFFSET_Y = 60;
    
    SDL_Color colorBackground = {192, 192, 192, 255};
    SDL_Color colorCell = {128, 128, 128, 255};
    SDL_Color colorRevealed = {255, 255, 255, 255};
    SDL_Color colorMine = {255, 0, 0, 255};
    SDL_Color colorFlag = {0, 255, 0, 255};
    SDL_Color colorText = {0, 0, 0, 255};
    
public:
    GameRenderer(int windowWidth, int windowHeight);
    ~GameRenderer();
    
    bool initialize();
    void cleanup();
    void render(const Minesweeper& game);
    void getCellFromMouse(int mouseX, int mouseY, int& cellX, int& cellY, const Minesweeper& game);
    
private:
    void renderCell(int x, int y, const Minesweeper& game);
    void renderText(const char* text, int x, int y, SDL_Color color);
};