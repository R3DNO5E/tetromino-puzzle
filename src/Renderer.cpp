#include "Renderer.h"
#include "TetrisGame.h"
#include <iostream>

Renderer::Renderer() : window(nullptr), renderer(nullptr), font(nullptr) {}

Renderer::~Renderer() {
    cleanup();
}

bool Renderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    
    window = SDL_CreateWindow("Tetris",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             WINDOW_WIDTH,
                             WINDOW_HEIGHT,
                             SDL_WINDOW_SHOWN);
    
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Try to load a system font, fallback to a simple implementation if not found
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
    if (!font) {
        font = TTF_OpenFont("/System/Library/Fonts/Arial.ttf", 16);
    }
    if (!font) {
        font = TTF_OpenFont("/Windows/Fonts/arial.ttf", 16);
    }
    if (!font) {
        std::cerr << "Warning: Could not load font! TTF_Error: " << TTF_GetError() << std::endl;
        std::cerr << "Text will be displayed as simple rectangles." << std::endl;
    }
    
    return true;
}

void Renderer::cleanup() {
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

void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}

void Renderer::setColor(const SDL_Color& color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Renderer::drawRect(int x, int y, int width, int height, bool filled) {
    SDL_Rect rect = {x, y, width, height};
    if (filled) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Renderer::drawGame(const TetrisGame& game) {
    clear();
    
    drawBoard(game);
    drawCurrentPiece(game);
    drawUI(game);
    
    present();
}

void Renderer::drawBoard(const TetrisGame& game) {
    const auto& board = game.getBoard();
    
    // Draw board background
    setColor({32, 32, 32, 255});
    drawRect(BOARD_OFFSET_X - 2, BOARD_OFFSET_Y - 2, 
             TetrisGame::BOARD_WIDTH * CELL_SIZE + 4, 
             TetrisGame::BOARD_HEIGHT * CELL_SIZE + 4);
    
    // Draw board border
    setColor({255, 255, 255, 255});
    drawRect(BOARD_OFFSET_X - 2, BOARD_OFFSET_Y - 2, 
             TetrisGame::BOARD_WIDTH * CELL_SIZE + 4, 
             TetrisGame::BOARD_HEIGHT * CELL_SIZE + 4, false);
    
    // Draw board cells
    for (int y = 0; y < TetrisGame::BOARD_HEIGHT; y++) {
        for (int x = 0; x < TetrisGame::BOARD_WIDTH; x++) {
            int cellValue = board[y][x];
            if (cellValue > 0) {
                setColor(colors[cellValue]);
                drawRect(BOARD_OFFSET_X + x * CELL_SIZE,
                        BOARD_OFFSET_Y + y * CELL_SIZE,
                        CELL_SIZE - 1, CELL_SIZE - 1);
            }
        }
    }
}

void Renderer::drawCurrentPiece(const TetrisGame& game) {
    if (game.isGameOver()) return;
    
    const auto& pieceShape = game.getCurrentPieceShape();
    int pieceType = static_cast<int>(game.getCurrentPieceType()) + 1; // +1 because colors[0] is for empty cells
    
    setColor(colors[pieceType]);
    
    for (int y = 0; y < TetrisGame::PIECE_SIZE; y++) {
        for (int x = 0; x < TetrisGame::PIECE_SIZE; x++) {
            if (pieceShape[y][x] == '#') {
                int screenX = BOARD_OFFSET_X + (game.getCurrentX() + x) * CELL_SIZE;
                int screenY = BOARD_OFFSET_Y + (game.getCurrentY() + y) * CELL_SIZE;
                
                if (screenX >= BOARD_OFFSET_X && 
                    screenX < BOARD_OFFSET_X + TetrisGame::BOARD_WIDTH * CELL_SIZE &&
                    screenY >= BOARD_OFFSET_Y && 
                    screenY < BOARD_OFFSET_Y + TetrisGame::BOARD_HEIGHT * CELL_SIZE) {
                    drawRect(screenX, screenY, CELL_SIZE - 1, CELL_SIZE - 1);
                }
            }
        }
    }
}

void Renderer::drawUI(const TetrisGame& game) {
    int infoX = BOARD_OFFSET_X + TetrisGame::BOARD_WIDTH * CELL_SIZE + 20;
    int infoY = BOARD_OFFSET_Y;
    
    setColor({255, 255, 255, 255});
    
    // Draw score, level, lines
    drawText("Score: " + std::to_string(game.getScore()), infoX, infoY);
    drawText("Level: " + std::to_string(game.getLevel()), infoX, infoY + 30);
    drawText("Lines: " + std::to_string(game.getLinesCleared()), infoX, infoY + 60);
    
    // Draw controls
    drawText("Controls:", infoX, infoY + 120);
    drawText("A/D - Move left/right", infoX, infoY + 150);
    drawText("S - Soft drop", infoX, infoY + 180);
    drawText("W - Rotate", infoX, infoY + 210);
    drawText("Space - Hard drop", infoX, infoY + 240);
    drawText("T - Toggle auto-play", infoX, infoY + 270);
    drawText("Q - Quit", infoX, infoY + 300);
    
    // Auto-play status
    std::string autoStatus = "Auto-play: " + std::string(game.isAutoPlay() ? "ON" : "OFF");
    drawText(autoStatus, infoX, infoY + 330);
    
    // Game over message
    if (game.isGameOver()) {
        setColor({255, 0, 0, 255});
        drawText("GAME OVER", BOARD_OFFSET_X + 50, BOARD_OFFSET_Y + 200);
        drawText("Press Q to quit", BOARD_OFFSET_X + 40, BOARD_OFFSET_Y + 230);
    }
}

void Renderer::drawText(const std::string& text, int x, int y) {
    if (font) {
        // Use SDL_ttf for proper text rendering
        SDL_Color color = {255, 255, 255, 255}; // White text
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};
                SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    } else {
        // Fallback: Simple text rendering using rectangles for characters
        const int charWidth = 8;
        const int charHeight = 12;
        
        setColor({255, 255, 255, 255}); // White color
        for (size_t i = 0; i < text.length(); i++) {
            char c = text[i];
            if (c >= 32 && c <= 126) { // Printable ASCII
                // Draw a simple rectangle for each character
                drawRect(x + i * charWidth, y, charWidth - 1, charHeight, false);
            }
        }
    }
}