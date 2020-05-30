#include "display.h"

#include "easylogging++.h"

Display::Display() {
    clear();
    LOG(INFO) << "Initialized Chip8 64x32 display";
};

Display::~Display() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
}

void Display::clear() {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            _display_data[y][x] = 0;
        }
    }
}

bool Display::drawPixel(bool pixel, uint8_t x, uint8_t y) {
    /**
     * XOR <pixel> into display data at coordinates <x> and <y>.
     * Return 1 if this causes the pixel to turn off, 0 otherwise.
     */
    bool turned_off = false;
    // If the pixel is on and the pixel at that location is also on, then they
    // collide.
    if (_display_data[y][x] && pixel) turned_off = true;
    _display_data[y][x] = _display_data[y][x] ^ pixel;
    return turned_off;
}

void Display::createWindow() {
    _window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                               WINDOW_HEIGHT, SDL_WINDOW_FLAGS);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_FLAGS);
    SDL_RenderSetLogicalSize(_renderer, RENDERER_WIDTH, RENDERER_HEIGHT);
    SDL_ShowCursor(SDL_FALSE);
}

void Display::draw() {
    SDL_SetRenderDrawColor(_renderer, primaryColor.r, primaryColor.g,
                           primaryColor.b, primaryColor.a);
    SDL_RenderClear(_renderer);
    SDL_SetRenderDrawColor(_renderer, secondaryColor.r, secondaryColor.g,
                           secondaryColor.b, secondaryColor.a);
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (_display_data[y][x]) SDL_RenderDrawPoint(_renderer, x, y);
        }
    }
    SDL_RenderPresent(_renderer);
}