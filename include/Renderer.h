#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class TetrisGame;

class Renderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 600;
    static const int CELL_SIZE = 25;
    static const int BOARD_OFFSET_X = 50;
    static const int BOARD_OFFSET_Y = 50;
    
    // Colors for different tetrominoes (index 0 = empty, 1-7 = I,O,T,S,Z,J,L)
    SDL_Color colors[8] = {
        {0, 0, 0, 255},       // 0: Empty
        {0, 255, 255, 255},   // 1: I piece - Cyan
        {255, 255, 0, 255},   // 2: O piece - Yellow  
        {128, 0, 128, 255},   // 3: T piece - Purple
        {0, 255, 0, 255},     // 4: S piece - Green
        {255, 0, 0, 255},     // 5: Z piece - Red
        {0, 0, 255, 255},     // 6: J piece - Blue
        {255, 165, 0, 255}    // 7: L piece - Orange
    };
    
public:
    Renderer();
    ~Renderer();
    
    bool initialize();
    void cleanup();
    
    void clear();
    void present();
    
    void drawGame(const TetrisGame& game);
    
private:
    void drawBoard(const TetrisGame& game);
    void drawCurrentPiece(const TetrisGame& game);
    void drawUI(const TetrisGame& game);
    void drawText(const std::string& text, int x, int y);
    void setColor(const SDL_Color& color);
    void drawRect(int x, int y, int width, int height, bool filled = true);
};