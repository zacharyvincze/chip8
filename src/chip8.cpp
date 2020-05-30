#include "chip8.h"

#include <chrono>
#include <fstream>
#include <nlohmann/json.hpp>
#include <streambuf>
#include <string>
#include <thread>

#include "easylogging++.h"

Chip8::Chip8(const std::string& filepath)
    : _mmu(),
      _cartridge(filepath, _mmu),
      _display(),
      _input(_event),
      _cpu(_mmu, _display, _input) {  // Initialization

    SDL_Init(SDL_INIT_EVERYTHING);
    _display.createWindow();

    // Read and set configurations
    std::ifstream t("config.json");
    std::string json_string;
    t.seekg(0, std::ios::end);
    json_string.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    json_string.assign((std::istreambuf_iterator<char>(t)),
                       std::istreambuf_iterator<char>());

    t.close();

    auto j = nlohmann::json::parse(json_string);

    _cpu.freq = j["cpu_frequency"];
    _display.primaryColor = {j["primary_color"]["r"], j["primary_color"]["g"],
                             j["primary_color"]["b"], 0};
    _display.secondaryColor = {j["secondary_color"]["r"],
                               j["secondary_color"]["g"],
                               j["secondary_color"]["b"], 0};

    if (j["fullscreen"]) _display.setFullscreen();

    LOG(INFO) << "Initialized Chip8 emulator";

    while (_running) {
        auto start_time = std::chrono::high_resolution_clock::now();
        // Run SDL Event Loop
        while (SDL_PollEvent(&_event)) {
            if (_event.type == SDL_QUIT) _running = false;
            if (_event.type == SDL_KEYDOWN) {
                if (_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    _running = false;
            }
            _input.pollEvents();
        }
        // _cpu.displayDebug();

        _cpu.tick();

        _display.draw();
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed_time = end_time - start_time;

        std::this_thread::sleep_for(std::chrono::milliseconds(
            1000 / _cpu.freq -
            std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time)
                .count()));
    }
}

Chip8::~Chip8() { LOG(INFO) << "Quit CHIP-8 emulator"; }