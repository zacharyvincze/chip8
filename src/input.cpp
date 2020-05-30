#include "input.h"

#include "easylogging++.h"

Input::Input(SDL_Event& event) : _event(event) {
    // Key  mapping
    _key_map[SDL_SCANCODE_1] = 1;
    _key_map[SDL_SCANCODE_2] = 2;
    _key_map[SDL_SCANCODE_3] = 3;
    _key_map[SDL_SCANCODE_4] = 0xC;
    _key_map[SDL_SCANCODE_Q] = 4;
    _key_map[SDL_SCANCODE_W] = 5;
    _key_map[SDL_SCANCODE_E] = 6;
    _key_map[SDL_SCANCODE_R] = 0xD;
    _key_map[SDL_SCANCODE_A] = 7;
    _key_map[SDL_SCANCODE_S] = 8;
    _key_map[SDL_SCANCODE_D] = 9;
    _key_map[SDL_SCANCODE_F] = 0xE;
    _key_map[SDL_SCANCODE_Z] = 0xA;
    _key_map[SDL_SCANCODE_X] = 0;
    _key_map[SDL_SCANCODE_C] = 0xB;
    _key_map[SDL_SCANCODE_V] = 0xF;

    for (int i = 0; i < 16; i++) {
        keys[i] = 0;
    }
}

void Input::pollEvents() {
    if (_event.type == SDL_KEYDOWN && _event.key.repeat == false) {
        if (_key_map.count(_event.key.keysym.scancode) != 0) {
            keys[_key_map[_event.key.keysym.scancode]] = 1;
        }
    } else if (_event.type == SDL_KEYUP) {
        if (_key_map.count(_event.key.keysym.scancode) != 0) {
            keys[_key_map[_event.key.keysym.scancode]] = 0;
        }
    }
}

uint8_t Input::waitUntilPress() {
    for (;;) {
        while (SDL_PollEvent(&_event)) {
            if (_event.type == SDL_KEYDOWN) {
                if (_key_map.count(_event.key.keysym.scancode) != 0) {
                    keys[_key_map[_event.key.keysym.scancode]] = 1;
                    return _key_map[_event.key.keysym.scancode];
                }
            }
        }
    }
}