#pragma once

/**
 * Main class responsible for combining the
 * components of the emulator
 **/

#include <string>

#include "cartridge.h"
#include "cpu.h"
#include "display.h"
#include "input.h"
#include "mmu.h"

class Chip8 {
    MMU _mmu;
    Cartridge _cartridge;
    CPU _cpu;
    Display _display;
    Input _input;

    bool _running = true;

    SDL_Event _event;

   public:
    Chip8(const std::string& filepath);
    ~Chip8();
};