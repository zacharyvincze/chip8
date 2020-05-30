#pragma once

#include "display.h"
#include "input.h"
#include "mmu.h"

/**
 * CPU module for the CHIP-8 emulator
 */

class CPU {
    MMU& _mmu;
    Display& _display;
    Input& _input;

    // Timers
    unsigned int _sound_timer, _delay_timer;

    // Registers
    uint8_t _V[16];
    uint16_t _I;
    uint16_t _pc;
    uint8_t _sp;
    uint16_t _stack[16];

    uint16_t fetchOpcode();
    void executeOpcode(uint16_t opcode);

   public:
    CPU(MMU& mmu, Display& display, Input& input);
    ~CPU();

    int freq;  // CPU Frequency (in Hz)

    void displayDebug();

    void tick();
};