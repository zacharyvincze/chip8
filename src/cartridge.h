#pragma once

/**
 * Responsible for loading cartridge data
 * into memory.
 */

#include <string>

#include "mmu.h"

class Cartridge {
    MMU& _mmu;

   public:
    Cartridge(const std::string& filepath, MMU& mmu);
};