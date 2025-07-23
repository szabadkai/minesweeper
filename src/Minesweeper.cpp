#include "Minesweeper.h"
#include <random>
#include <chrono>

Minesweeper::Minesweeper(int w, int h, int m) : width(w), height(h), mines(m), gameOver(false), gameWon(false) {
    board.resize(height, std::vector<int>(width, 0));
    revealed.resize(height, std::vector<bool>(width, false));
    flagged.resize(height, std::vector<bool>(width, false));
    initializeBoard();
}

void Minesweeper::initializeBoard() {
    // Clear previous state
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            board[y][x] = 0;
            revealed[y][x] = false;
            flagged[y][x] = false;
        }
    }
    
    gameOver = false;
    gameWon = false;
    
    // Place mines randomly
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, width - 1);
    std::uniform_int_distribution<> disY(0, height - 1);
    
    for (int i = 0; i < mines; ++i) {
        int x, y;
        do {
            x = disX(gen);
            y = disY(gen);
        } while (board[y][x] == -1);
        
        board[y][x] = -1; // -1 represents a mine
    }
    
    // Calculate numbers for non-mine cells
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (board[y][x] != -1) {
                board[y][x] = countAdjacentMines(x, y);
            }
        }
    }
}

int Minesweeper::countAdjacentMines(int x, int y) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && board[ny][nx] == -1) {
                count++;
            }
        }
    }
    return count;
}

void Minesweeper::reveal(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height || revealed[y][x] || flagged[y][x]) {
        return;
    }
    
    revealed[y][x] = true;
    
    if (board[y][x] == -1) {
        gameOver = true;
        // Reveal all mines when game is over
        for (int py = 0; py < height; ++py) {
            for (int px = 0; px < width; ++px) {
                if (board[py][px] == -1) {
                    revealed[py][px] = true;
                }
            }
        }
        return;
    }
    
    if (board[y][x] == 0) {
        // Reveal adjacent cells if this cell has no adjacent mines
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                reveal(x + dx, y + dy);
            }
        }
    }
    
    checkWin();
}

void Minesweeper::toggleFlag(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height && !revealed[y][x]) {
        flagged[y][x] = !flagged[y][x];
    }
}

void Minesweeper::checkWin() {
    int revealedCount = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (revealed[y][x]) {
                revealedCount++;
            }
        }
    }
    
    if (revealedCount == width * height - mines) {
        gameWon = true;
    }
}

bool Minesweeper::isRevealed(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return revealed[y][x];
}

bool Minesweeper::isFlagged(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return flagged[y][x];
}

bool Minesweeper::isMine(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return false;
    return board[y][x] == -1;
}

int Minesweeper::getNumber(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return 0;
    return board[y][x];
}

void Minesweeper::reset() {
    initializeBoard();
}