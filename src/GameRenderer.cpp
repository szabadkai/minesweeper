#include "GameRenderer.h"
#include <iostream>
#include <string>
#include <cstring>

GameRenderer::GameRenderer(int windowWidth, int windowHeight) 
    : window(nullptr), renderer(nullptr), font(nullptr) {
    
    window = SDL_CreateWindow("Minesweeper", 
                             SDL_WINDOWPOS_CENTERED, 
                             SDL_WINDOWPOS_CENTERED,
                             windowWidth, windowHeight, 
                             SDL_WINDOW_SHOWN);
}

GameRenderer::~GameRenderer() {
    cleanup();
}

bool GameRenderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Try to load system fonts with multiple fallbacks
    const char* fontPaths[] = {
        "/System/Library/Fonts/Helvetica.ttc",               // macOS - Helvetica 
        "/System/Library/Fonts/HelveticaNeue.ttc",           // macOS - Helvetica Neue
        "/System/Library/Fonts/Times.ttc",                   // macOS - Times
        "/System/Library/Fonts/ArialHB.ttc",                 // macOS - Arial Hebrew Bold
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",   // Linux
        "/usr/share/fonts/TTF/DejaVuSans.ttf",               // Arch Linux
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", // Linux fallback
        nullptr
    };
    
    for (int i = 0; fontPaths[i] != nullptr; ++i) {
        font = TTF_OpenFont(fontPaths[i], 18);
        if (font) {
            std::cout << "Successfully loaded font: " << fontPaths[i] << std::endl;
            break;
        } else {
            std::cout << "Failed to load font: " << fontPaths[i] << " (" << TTF_GetError() << ")" << std::endl;
        }
    }
    
    if (!font) {
        std::cerr << "Failed to load any font! Numbers will be displayed as colored squares instead." << std::endl;
    }
    
    return true;
}

void GameRenderer::cleanup() {
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
    SDL_Quit();
}

void GameRenderer::render(const Minesweeper& game) {
    // Clear screen with background color
    SDL_SetRenderDrawColor(renderer, colorBackground.r, colorBackground.g, colorBackground.b, colorBackground.a);
    SDL_RenderClear(renderer);
    
    // Render game status
    if (game.isGameWon()) {
        renderText("*** CONGRATULATIONS! YOU WIN! ***", 20, 20, {0, 255, 0, 255});
        renderText("Press 'R' to play again", 20, 40, {0, 200, 0, 255});
    } else if (game.isGameOver()) {
        renderText("GAME OVER! Mine hit!", 20, 20, {255, 0, 0, 255});
        renderText("Press 'R' to restart", 20, 40, {200, 0, 0, 255});
    } else {
        renderText("Left click: reveal, Right click: flag", 20, 20, colorText);
    }
    
    // Render grid
    for (int y = 0; y < game.getHeight(); ++y) {
        for (int x = 0; x < game.getWidth(); ++x) {
            renderCell(x, y, game);
        }
    }
    
    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

void GameRenderer::renderCell(int x, int y, const Minesweeper& game) {
    int screenX = GRID_OFFSET_X + x * CELL_SIZE;
    int screenY = GRID_OFFSET_Y + y * CELL_SIZE;
    
    SDL_Rect cellRect = {screenX, screenY, CELL_SIZE - 1, CELL_SIZE - 1};
    
    if (game.isRevealed(x, y)) {
        // Revealed cell
        SDL_SetRenderDrawColor(renderer, colorRevealed.r, colorRevealed.g, colorRevealed.b, colorRevealed.a);
        SDL_RenderFillRect(renderer, &cellRect);
        
        if (game.isMine(x, y)) {
            // Draw mine as red circle
            SDL_SetRenderDrawColor(renderer, colorMine.r, colorMine.g, colorMine.b, colorMine.a);
            SDL_RenderFillRect(renderer, &cellRect);
        } else {
            int number = game.getNumber(x, y);
            if (number > 0) {
                // Use traditional minesweeper colors for numbers
                SDL_Color numberColors[] = {
                    {0, 0, 0, 255},       // 0 (unused)
                    {0, 0, 255, 255},     // 1 - blue
                    {0, 128, 0, 255},     // 2 - green
                    {255, 0, 0, 255},     // 3 - red
                    {0, 0, 128, 255},     // 4 - dark blue
                    {128, 0, 0, 255},     // 5 - maroon
                    {0, 128, 128, 255},   // 6 - teal
                    {0, 0, 0, 255},       // 7 - black
                    {128, 128, 128, 255}  // 8 - gray
                };
                
                SDL_Color numColor = (number <= 8) ? numberColors[number] : colorText;
                std::string numStr = std::to_string(number);
                renderText(numStr.c_str(), screenX + 10, screenY + 8, numColor);
            }
        }
    } else {
        // Unrevealed cell
        SDL_SetRenderDrawColor(renderer, colorCell.r, colorCell.g, colorCell.b, colorCell.a);
        SDL_RenderFillRect(renderer, &cellRect);
        
        if (game.isFlagged(x, y)) {
            // Draw flag as green square
            SDL_Rect flagRect = {screenX + 8, screenY + 8, CELL_SIZE - 16, CELL_SIZE - 16};
            SDL_SetRenderDrawColor(renderer, colorFlag.r, colorFlag.g, colorFlag.b, colorFlag.a);
            SDL_RenderFillRect(renderer, &flagRect);
        }
    }
    
    // Draw cell border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &cellRect);
}

void GameRenderer::renderText(const char* text, int x, int y, SDL_Color color) {
    if (!font) {
        // Fallback: render as colored squares for numbers 1-8
        if (strlen(text) == 1 && text[0] >= '1' && text[0] <= '8') {
            int num = text[0] - '0';
            // Use different colors for different numbers
            SDL_Color numColors[] = {
                {0, 0, 0, 255},       // 0 (unused)
                {0, 0, 255, 255},     // 1 - blue
                {0, 128, 0, 255},     // 2 - green  
                {255, 0, 0, 255},     // 3 - red
                {128, 0, 128, 255},   // 4 - purple
                {128, 0, 0, 255},     // 5 - maroon
                {0, 128, 128, 255},   // 6 - teal
                {0, 0, 0, 255},       // 7 - black
                {128, 128, 128, 255}  // 8 - gray
            };
            
            SDL_SetRenderDrawColor(renderer, numColors[num].r, numColors[num].g, numColors[num].b, numColors[num].a);
            
            // Draw small squares to represent the number
            for (int i = 0; i < num && i < 4; ++i) {  // Max 4 squares to fit in cell
                int squareSize = 3;
                int offsetX = (i % 2) * 6;
                int offsetY = (i / 2) * 6;
                SDL_Rect numRect = {x + 6 + offsetX, y + 6 + offsetY, squareSize, squareSize};
                SDL_RenderFillRect(renderer, &numRect);
            }
        }
        return;
    }
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface) return;
    
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture) {
        SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
        SDL_DestroyTexture(textTexture);
    }
    
    SDL_FreeSurface(textSurface);
}

void GameRenderer::getCellFromMouse(int mouseX, int mouseY, int& cellX, int& cellY, const Minesweeper& game) {
    cellX = (mouseX - GRID_OFFSET_X) / CELL_SIZE;
    cellY = (mouseY - GRID_OFFSET_Y) / CELL_SIZE;
    
    // Clamp to valid range
    if (cellX < 0) cellX = -1;
    if (cellY < 0) cellY = -1;
    if (cellX >= game.getWidth()) cellX = -1;
    if (cellY >= game.getHeight()) cellY = -1;
}