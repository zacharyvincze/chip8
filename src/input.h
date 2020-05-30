#pragma once

#include <SDL2/SDL.h>

#include <map>

class Input {
   private:
    SDL_Event& _event;
    std::map<SDL_Scancode, uint8_t> _key_map;

   public:
    bool keys[16];

    Input(SDL_Event& event);

    void pollEvents();
    uint8_t waitUntilPress();
};