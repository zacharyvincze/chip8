#pragma once
/**
 * CHIP-8 display module.
 * Responsible for handling and initializing graphics.
 */

#include <vector>

#include "SDL2/SDL.h"

const unsigned int WINDOW_WIDTH = 64 * 8;
const unsigned int WINDOW_HEIGHT = 32 * 8;
const unsigned int RENDERER_WIDTH = 64;
const unsigned int RENDERER_HEIGHT = 32;

const uint32_t SDL_WINDOW_FLAGS = SDL_WINDOW_SHOWN;
const uint32_t SDL_RENDERER_FLAGS = SDL_RENDERER_ACCELERATED;

class Display {
    bool _display_data[32][64];

    SDL_Window* _window;
    SDL_Renderer* _renderer;

   public:
    Display();
    ~Display();

    bool drawPixel(bool pixel, uint8_t x, uint8_t y);

    SDL_Color primaryColor;
    SDL_Color secondaryColor;

    inline void setFullscreen() {
        SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    void createWindow();
    void clear();
    void draw();
};