#include "BlockDropGame.h"
#include <algorithm>
#include <chrono>

BlockDropGame::BlockDropGame() 
    : board(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0))
    , currentPiece(TetrominoType::I)
    , currentX(0), currentY(0), currentRotation(0)
    , score(0), level(1), linesCleared(0), gameOver(false)
    , fallTime(0.0), fallSpeed(0.5), autoPlay(false), autoPlayPositioned(false)
    , targetX(0), targetRotation(0), autoTimer(0.0)
    , rng(std::chrono::steady_clock::now().time_since_epoch().count())
{
    initializeTetrominoes();
    newPiece();
}

void BlockDropGame::initializeTetrominoes() {
    // I piece
    tetrominoes[0].rotations = {
        {".....", "..#..", "..#..", "..#..", "..#.."},
        {".....", ".....", "####.", ".....", "....."}
    };
    
    // O piece
    tetrominoes[1].rotations = {
        {".....", ".....", ".##..", ".##..", "....."}
    };
    
    // T piece
    tetrominoes[2].rotations = {
        {".....", ".....", ".#...", "###..", "....."},
        {".....", ".....", ".#...", ".##..", ".#..."},
        {".....", ".....", ".....", "###..", ".#..."},
        {".....", ".....", ".#...", "##...", ".#..."}
    };
    
    // S piece
    tetrominoes[3].rotations = {
        {".....", ".....", ".##..", "##...", "....."},
        {".....", ".#...", ".##..", "..#..", "....."}
    };
    
    // Z piece
    tetrominoes[4].rotations = {
        {".....", ".....", "##...", ".##..", "....."},
        {".....", "..#..", ".##..", ".#...", "....."}
    };
    
    // J piece
    tetrominoes[5].rotations = {
        {".....", ".#...", ".#...", "##...", "....."},
        {".....", ".....", "#....", "###..", "....."},
        {".....", ".##..", ".#...", ".#...", "....."},
        {".....", ".....", "###..", "..#..", "....."}
    };
    
    // L piece
    tetrominoes[6].rotations = {
        {".....", "..#..", "..#..", ".##..", "....."},
        {".....", ".....", "###..", "#....", "....."},
        {".....", "##...", ".#...", ".#...", "....."},
        {".....", ".....", "..#..", "###..", "....."}
    };
}

void BlockDropGame::newPiece() {
    std::uniform_int_distribution<int> dist(0, static_cast<int>(TetrominoType::COUNT) - 1);
    currentPiece = static_cast<TetrominoType>(dist(rng));
    currentRotation = 0;
    currentX = BOARD_WIDTH / 2 - 2;
    currentY = 0;
    autoPlayPositioned = false;  // Reset positioning state for new piece
    autoTimer = 0.0;  // Reset auto-play timer
    
    // Calculate target position for this new piece
    if (autoPlay) {
        auto [bestX, bestRotation] = getBestMove();
        targetX = bestX;
        targetRotation = bestRotation;
    }
    
    if (checkCollision()) {
        gameOver = true;
    }
}

const std::array<std::string, BlockDropGame::PIECE_SIZE>& BlockDropGame::getCurrentPieceShape() const {
    return tetrominoes[static_cast<int>(currentPiece)].rotations[currentRotation];
}

bool BlockDropGame::checkCollision(int dx, int dy, int rotation) const {
    if (rotation == -1) {
        rotation = currentRotation;
    }
    
    const auto& pieceShape = tetrominoes[static_cast<int>(currentPiece)].rotations[rotation];
    
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            if (pieceShape[y][x] == '#') {
                int newX = currentX + x + dx;
                int newY = currentY + y + dy;
                
                if (newX < 0 || newX >= BOARD_WIDTH || 
                    newY >= BOARD_HEIGHT ||
                    (newY >= 0 && board[newY][newX])) {
                    return true;
                }
            }
        }
    }
    return false;
}

void BlockDropGame::placePiece() {
    const auto& pieceShape = getCurrentPieceShape();
    
    for (int y = 0; y < PIECE_SIZE; y++) {
        for (int x = 0; x < PIECE_SIZE; x++) {
            if (pieceShape[y][x] == '#') {
                int boardX = currentX + x;
                int boardY = currentY + y;
                if (boardY >= 0) {
                    board[boardY][boardX] = static_cast<int>(currentPiece) + 1;
                }
            }
        }
    }
}

void BlockDropGame::clearLines() {
    std::vector<int> linesToClear;
    
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        bool fullLine = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x] == 0) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            linesToClear.push_back(y);
        }
    }
    
    for (int y : linesToClear) {
        board.erase(board.begin() + y);
        board.insert(board.begin(), std::vector<int>(BOARD_WIDTH, 0));
    }
    
    int clearedCount = linesToClear.size();
    linesCleared += clearedCount;
    
    if (clearedCount > 0) {
        score += clearedCount * 100 * level;
        level = linesCleared / 10 + 1;
        fallSpeed = std::max(0.1, 0.5 - (level - 1) * 0.05);
    }
}

bool BlockDropGame::movePiece(int dx, int dy) {
    if (!checkCollision(dx, dy)) {
        currentX += dx;
        currentY += dy;
        return true;
    }
    return false;
}

void BlockDropGame::rotatePiece() {
    int newRotation = (currentRotation + 1) % tetrominoes[static_cast<int>(currentPiece)].rotations.size();
    if (!checkCollision(0, 0, newRotation)) {
        currentRotation = newRotation;
    }
}

void BlockDropGame::dropPiece() {
    if (!movePiece(0, 1)) {
        placePiece();
        clearLines();
        newPiece();
    }
}

void BlockDropGame::hardDrop() {
    while (movePiece(0, 1)) {
        // Keep dropping
    }
    dropPiece();
}

int BlockDropGame::getGhostY() const {
    int ghostY = currentY;
    while (!checkCollision(0, ghostY - currentY + 1)) {
        ghostY++;
    }
    return ghostY;
}

double BlockDropGame::evaluateBoard() const {
    double score = 0.0;
    
    // Calculate column heights
    std::vector<int> heights(BOARD_WIDTH, 0);
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (board[y][x]) {
                heights[x] = BOARD_HEIGHT - y;
                break;
            }
        }
    }
    
    // Maximum height penalty (CRITICAL - avoid game over)
    int maxHeight = *std::max_element(heights.begin(), heights.end());
    if (maxHeight > 18) {
        score -= 1000.0;  // Extreme penalty for dangerous heights
    } else if (maxHeight > 15) {
        score -= (maxHeight - 15) * 50.0;  // Heavy penalty for risky heights
    } else if (maxHeight > 12) {
        score -= (maxHeight - 12) * 10.0;  // Moderate penalty for medium heights
    }
    
    // Aggregate height penalty (increased for safety)
    int aggregateHeight = 0;
    for (int height : heights) {
        aggregateHeight += height;
    }
    score -= aggregateHeight * 0.6;  // Increased penalty to keep board lower
    
    // Complete lines bonus (linear, not exponential for safety)
    int completeLines = 0;
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        bool complete = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (!board[y][x]) {
                complete = false;
                break;
            }
        }
        if (complete) completeLines++;
    }
    // Linear bonus - prioritize any line clear over risky play
    score += completeLines * 10.0;  // Good bonus but not overwhelming
    
    // Holes penalty (very severe - holes are dangerous)
    int holes = 0;
    for (int x = 0; x < BOARD_WIDTH; x++) {
        bool foundBlock = false;
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            if (board[y][x]) {
                foundBlock = true;
            } else if (foundBlock) {
                holes++;
            }
        }
    }
    score -= holes * 5.0;  // Very high penalty for holes
    
    // Bumpiness penalty (increased for safety)
    int bumpiness = 0;
    for (int x = 0; x < BOARD_WIDTH - 1; x++) {
        bumpiness += std::abs(heights[x] - heights[x + 1]);
    }
    score -= bumpiness * 0.8;  // Higher penalty for uneven surface
    
    // Safety bonus for low, flat board
    if (maxHeight <= 8) {
        score += 5.0;  // Bonus for keeping board low
    }
    if (bumpiness <= 3) {
        score += 3.0;  // Bonus for flat surface
    }
    
    // Count total tiles on board (encourage clearing)
    int totalTiles = 0;
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (board[y][x]) {
                totalTiles++;
            }
        }
    }
    score -= totalTiles * 0.05;  // Small penalty for tiles
    
    // Bonus for almost complete lines (but only if safe)
    if (maxHeight <= 15) {  // Only encourage this if board is safe
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            int filledCells = 0;
            for (int x = 0; x < BOARD_WIDTH; x++) {
                if (board[y][x]) {
                    filledCells++;
                }
            }
            if (filledCells >= 9) {  // Only very close to complete
                score += 1.0;  // Small bonus for nearly complete lines
            }
        }
    }
    
    // Penalty for empty columns
    for (int x = 0; x < BOARD_WIDTH; x++) {
        bool columnEmpty = true;
        for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
            if (board[y][x]) {
                columnEmpty = false;
                break;
            }
        }
        if (columnEmpty && maxHeight > 5) {
            // Penalty for empty columns when there's significant height
            score -= 1.0;  // Equal penalty for all empty columns
        }
    }
    
    return score;
}

std::pair<int, int> BlockDropGame::getBestMove() const {
    double bestScore = -1e9;
    int bestX = currentX;
    int bestRotation = currentRotation;
    
    // Save current state
    int oldX = currentX, oldY = currentY, oldRotation = currentRotation;
    auto oldBoard = board;
    
    for (size_t rotation = 0; rotation < tetrominoes[static_cast<int>(currentPiece)].rotations.size(); rotation++) {
        // Expand search range to include positions where piece extends beyond left edge
        for (int x = -2; x <= BOARD_WIDTH + 1; x++) {
            // Simulate placement
            const_cast<BlockDropGame*>(this)->currentX = x;
            const_cast<BlockDropGame*>(this)->currentRotation = rotation;
            const_cast<BlockDropGame*>(this)->currentY = 0;
            
            // Check if this position is valid at the top
            if (!checkCollision()) {
                int ghostY = getGhostY();
                const_cast<BlockDropGame*>(this)->currentY = ghostY;
                
                // Final collision check at ghost position
                if (!checkCollision()) {
                    const_cast<BlockDropGame*>(this)->placePiece();
                    double score = evaluateBoard();
                    
                    if (score > bestScore) {
                        bestScore = score;
                        bestX = x;
                        bestRotation = rotation;
                    }
                    
                    // Restore board
                    const_cast<BlockDropGame*>(this)->board = oldBoard;
                }
            }
        }
    }
    
    // If no valid move found, try to find any valid position
    if (bestScore == -1e9) {
        for (size_t rotation = 0; rotation < tetrominoes[static_cast<int>(currentPiece)].rotations.size(); rotation++) {
            for (int x = -2; x <= BOARD_WIDTH + 1; x++) {
                const_cast<BlockDropGame*>(this)->currentX = x;
                const_cast<BlockDropGame*>(this)->currentRotation = rotation;
                const_cast<BlockDropGame*>(this)->currentY = 0;
                
                if (!checkCollision()) {
                    bestX = x;
                    bestRotation = rotation;
                    break;
                }
            }
            if (bestScore != -1e9) break;
        }
    }
    
    // Restore state
    const_cast<BlockDropGame*>(this)->currentX = oldX;
    const_cast<BlockDropGame*>(this)->currentY = oldY;
    const_cast<BlockDropGame*>(this)->currentRotation = oldRotation;
    
    return {bestX, bestRotation};
}

void BlockDropGame::autoPlayStep() {
    if (gameOver || autoPlayPositioned) return;  // Don't adjust if already positioned
    
    // Verify target position is still valid
    int oldX = currentX, oldY = currentY, oldRotation = currentRotation;
    currentX = targetX;
    currentRotation = targetRotation;
    
    if (checkCollision()) {
        // Target position is no longer valid, recalculate
        currentX = oldX;
        currentY = oldY;
        currentRotation = oldRotation;
        auto [newX, newRotation] = getBestMove();
        targetX = newX;
        targetRotation = newRotation;
    } else {
        // Restore current position
        currentX = oldX;
        currentY = oldY;
        currentRotation = oldRotation;
    }
    
    // Use cached target position to avoid recalculation
    if (currentRotation != targetRotation) {
        rotatePiece();
    } else if (currentX < targetX) {
        if (!movePiece(1, 0)) {
            // Can't move right, consider positioned
            autoPlayPositioned = true;
        }
    } else if (currentX > targetX) {
        if (!movePiece(-1, 0)) {
            // Can't move left, consider positioned
            autoPlayPositioned = true;
        }
    } else {
        // Position and rotation are correct, mark as positioned for faster falling
        autoPlayPositioned = true;
    }
}

void BlockDropGame::update(double deltaTime) {
    if (gameOver) return;
    
    if (autoPlay && !autoPlayPositioned) {
        autoTimer += deltaTime;
        if (autoTimer >= 0.1) {
            autoPlayStep();
            autoTimer = 0.0;
        }
        // Do not update fallTime when positioning in auto-play mode
        return;
    }
    
    fallTime += deltaTime;
    double currentFallSpeed;
    
    if (autoPlay) {
        if (autoPlayPositioned) {
            // Fast fall when position is finalized
            currentFallSpeed = 0.02;
        } else {
            // This branch should not be reached due to early return above
            currentFallSpeed = 0.05;
        }
    } else {
        // Manual play speed
        currentFallSpeed = fallSpeed;
    }
    
    if (fallTime >= currentFallSpeed) {
        dropPiece(); // Both manual and auto-play use natural falling
        fallTime = 0.0;
    }
}

void BlockDropGame::handleInput(char key) {
    if (gameOver) return;
    
    if (key == 't' || key == 'T') {
        toggleAutoPlay();
        return;
    }
    
    if (autoPlay) return;
    
    switch (key) {
        case 'a': case 'A':
            movePiece(-1, 0);
            break;
        case 'd': case 'D':
            movePiece(1, 0);
            break;
        case 's': case 'S':
            dropPiece();
            break;
        case 'w': case 'W':
            rotatePiece();
            break;
        case ' ':
            hardDrop();
            break;
    }
}