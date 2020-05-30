#include "mmu.h"

MMU::MMU() {
    _memory.resize(CHIP8_MEMORY_CAPACITY);

    for (int i = 0; i < 0x1000; i++) {
        _memory[i] = 0;
    }

    for (int i = 0; i < 80; i++) {
        _memory[i] = CHARACTER_SPRITES[i];
    }

    LOG(INFO) << "Initialized Chip8 memory (" << CHIP8_MEMORY_CAPACITY
              << " bytes)";
}