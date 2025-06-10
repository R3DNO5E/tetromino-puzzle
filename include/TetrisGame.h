#pragma once
#include <vector>
#include <array>
#include <random>

class TetrisGame {
public:
    static const int BOARD_WIDTH = 10;
    static const int BOARD_HEIGHT = 20;
    static const int PIECE_SIZE = 5;
    
    enum class TetrominoType {
        I = 0, O, T, S, Z, J, L, COUNT
    };
    
    struct Tetromino {
        std::vector<std::array<std::string, PIECE_SIZE>> rotations;
    };

private:
    std::vector<std::vector<int>> board;
    std::array<Tetromino, static_cast<int>(TetrominoType::COUNT)> tetrominoes;
    
    TetrominoType currentPiece;
    int currentX, currentY;
    int currentRotation;
    
    int score;
    int level;
    int linesCleared;
    bool gameOver;
    
    double fallTime;
    double fallSpeed;
    bool autoPlay;
    bool autoPlayPositioned;  // True when auto-play has reached target position and rotation
    int targetX, targetRotation;  // Cache target position to avoid recalculation
    double autoTimer;  // Timer for auto-play step intervals
    
    std::mt19937 rng;
    
public:
    TetrisGame();
    
    void newPiece();
    bool checkCollision(int dx = 0, int dy = 0, int rotation = -1) const;
    void placePiece();
    void clearLines();
    bool movePiece(int dx, int dy);
    void rotatePiece();
    void dropPiece();
    void hardDrop();
    
    void update(double deltaTime);
    void handleInput(char key);
    
    // Getters
    const std::vector<std::vector<int>>& getBoard() const { return board; }
    const std::array<std::string, PIECE_SIZE>& getCurrentPieceShape() const;
    TetrominoType getCurrentPieceType() const { return currentPiece; }
    int getCurrentX() const { return currentX; }
    int getCurrentY() const { return currentY; }
    int getScore() const { return score; }
    int getLevel() const { return level; }
    int getLinesCleared() const { return linesCleared; }
    bool isGameOver() const { return gameOver; }
    bool isAutoPlay() const { return autoPlay; }
    
    void toggleAutoPlay() { autoPlay = !autoPlay; }
    
private:
    void initializeTetrominoes();
    int getGhostY() const;
    double evaluateBoard() const;
    std::pair<int, int> getBestMove() const;
    void autoPlayStep();
};