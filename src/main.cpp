#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include "TetrisGame.h"
#include "Renderer.h"

int main() {
    Renderer renderer;
    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return 1;
    }
    
    TetrisGame game;
    
    bool running = true;
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                
                if (key == SDLK_q) {
                    running = false;
                } else {
                    char inputChar = 0;
                    
                    switch (key) {
                        case SDLK_a: inputChar = 'a'; break;
                        case SDLK_d: inputChar = 'd'; break;
                        case SDLK_s: inputChar = 's'; break;
                        case SDLK_w: inputChar = 'w'; break;
                        case SDLK_SPACE: inputChar = ' '; break;
                        case SDLK_t: inputChar = 't'; break;
                        case SDLK_LEFT: inputChar = 'a'; break;
                        case SDLK_RIGHT: inputChar = 'd'; break;
                        case SDLK_DOWN: inputChar = 's'; break;
                        case SDLK_UP: inputChar = 'w'; break;
                    }
                    
                    if (inputChar != 0) {
                        game.handleInput(inputChar);
                    }
                }
            }
        }
        
        game.update(deltaTime);
        renderer.drawGame(game);
        
        SDL_Delay(16); // ~60 FPS
    }
    
    return 0;
}