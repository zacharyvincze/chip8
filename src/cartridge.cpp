#include "cartridge.h"

#include <fstream>
#include <vector>

#include "easylogging++.h"

Cartridge::Cartridge(const std::string& filepath, MMU& mmu) : _mmu(mmu) {
    std::ifstream file;
    file.open(filepath.c_str());
    if (!file.is_open()) {
        LOG(ERROR) << "Unable to load cartridge data into memory: " << filepath;
        exit(EXIT_FAILURE);
    }

    size_t begin = file.tellg();
    file.seekg(0, std::ios::end);
    size_t end = file.tellg();
    file.seekg(0, std::ios::beg);
    size_t filesize = end - begin;

    std::vector<char> data(filesize);
    file.read(data.data(), filesize);

    for (int i = 0; i < filesize; i++) {
        _mmu.writeByte(0x200 + i, data[i]);
    }

    LOG(INFO) << "Successfully loaded cartridge into memory (" << filesize
              << " bytes)";

    file.close();
}