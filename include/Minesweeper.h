#pragma once

#include <vector>
#include <SDL.h>

class Minesweeper {
private:
    int width, height, mines;
    std::vector<std::vector<int>> board;
    std::vector<std::vector<bool>> revealed;
    std::vector<std::vector<bool>> flagged;
    bool gameOver;
    bool gameWon;
    
public:
    Minesweeper(int w, int h, int m);
    ~Minesweeper() = default;
    
    void initializeBoard();
    int countAdjacentMines(int x, int y);
    void reveal(int x, int y);
    void toggleFlag(int x, int y);
    void checkWin();
    
    bool isGameOver() const { return gameOver; }
    bool isGameWon() const { return gameWon; }
    bool isRevealed(int x, int y) const;
    bool isFlagged(int x, int y) const;
    bool isMine(int x, int y) const;
    int getNumber(int x, int y) const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    void reset();
};