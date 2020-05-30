#include "cpu.h"

#include <stdlib.h>

#include "easylogging++.h"

CPU::CPU(MMU& mmu, Display& display, Input& input)
    : _mmu(mmu), _display(display), _input(input) {
    _delay_timer = 0;
    _sound_timer = 0;

    for (int i = 0; i < 16; i++) {
        _V[i] = 0;
        _stack[i] = 0;
    }

    _I = 0;
    _pc = 0x200;
    _sp = 0;

    LOG(INFO) << "Initialized Chip8 CPU";
}

CPU::~CPU() {}

void CPU::displayDebug() {
    printf("PC: %04X\n", _pc);
    for (int i = 0; i < 16; i++) {
        printf("V[%02X]: %02X\tS[%02X]: %02X\t K[%02X]: %i\n", i, _V[i], i,
               _stack[i], i, _input.keys[i]);
    }
    printf("I: %04X\t DT: %i\t DS: %i\n", _I, _delay_timer, _sound_timer);
}

void CPU::tick() {
    uint16_t opcode = fetchOpcode();
    executeOpcode(opcode);
    if (_delay_timer != 0) _delay_timer -= 1;
    if (_sound_timer != 0) _sound_timer -= 1;
}

uint16_t CPU::fetchOpcode() {
    uint16_t opcode = _mmu.readByte(_pc) << 8 | _mmu.readByte(_pc + 1);
    _pc += 2;
    return opcode;
}

void CPU::executeOpcode(uint16_t opcode) {
    uint8_t x = opcode >> 8 & 0xF;
    uint8_t y = opcode >> 4 & 0xF;

    switch (opcode >> 12) {
        case 0x0:
            switch (opcode & 0xF) {
                case 0x0:
                    _display.clear();
                    break;
                case 0xE:
                    _pc = _stack[_sp];
                    _sp--;
                    break;
            }
            break;
        case 0x1:
            _pc = (opcode & 0xFFF);
            break;
        case 0x2:
            _sp++;
            _stack[_sp] = _pc;
            _pc = (opcode & 0xFFF);
            break;

        case 0x3:
            if (_V[x] == (opcode & 0xFF)) _pc += 2;
            break;
        case 0x4:
            if (_V[x] != (opcode & 0xFF)) _pc += 2;
            break;
        case 0x5:
            if (_V[x] == _V[y]) _pc += 2;
            break;
        case 0x6:
            _V[x] = (opcode & 0xFF);
            break;
        case 0x7:
            _V[x] += (opcode & 0xFF);
            break;
        case 0x8:
            switch (opcode & 0xF) {
                case 0:
                    _V[x] = _V[y];
                    break;
                case 1:
                    _V[x] = _V[x] | _V[y];
                    break;
                case 2:
                    _V[x] = _V[x] & _V[y];
                    break;
                case 3:
                    _V[x] = _V[x] ^ _V[y];
                    break;
                case 4: {
                    unsigned int sum = _V[x] + _V[y];
                    sum > 255 ? _V[0xF] = 1 : _V[0xF] = 0;
                    _V[x] = sum & 0xFF;
                    break;
                }
                case 5: {
                    _V[x] > _V[y] ? _V[0xF] = 1 : _V[0xF] = 0;
                    _V[x] = _V[x] - _V[y];
                    break;
                }
                case 6: {
                    _V[x] & 1 ? _V[0xF] = 1 : _V[0xF] = 0;
                    _V[x] /= 2;
                    break;
                }
                case 7: {
                    _V[y] > _V[x] ? _V[0xF] = 1 : _V[0xF] = 0;
                    _V[x] = _V[y] - _V[x];
                    break;
                }
                case 0xE: {
                    _V[0xF] = (_V[x] >> 7) & 1;
                    _V[x] *= 2;
                    break;
                }
            }
            break;
        case 0x9:
            switch (opcode & 0xF) {
                case 0:
                    if (_V[x] != _V[y]) _pc += 2;
                    break;
            }
            break;
        case 0xA:
            _I = opcode & 0xFFF;
            break;
        case 0xB:
            _pc = (opcode & 0xFFF) + _V[0];
            break;
        case 0xC:
            _V[x] = (rand() & 0xFF) & (opcode & 0xFF);
            break;
        case 0xD: {
            uint8_t n = opcode & 0xF;
            uint8_t pos_x = _V[x];
            uint8_t pos_y = _V[y];
            bool turned_off = false;
            for (int i = 0; i < n; i++) {
                uint8_t sprite_data = _mmu.readByte(_I + i);
                // Draw this sprite as a line on the screen
                // Reset x position to start a new line
                pos_x = _V[x];
                for (int j = 7; j >= 0; j--) {
                    if (_display.drawPixel(sprite_data >> j & 1, pos_x, pos_y))
                        turned_off = true;
                    // Increment the x position to draw
                    pos_x++;
                    if (pos_x >= 64) pos_x = 0;
                }
                // Increment the y position to start the next line
                pos_y++;
                if (pos_y >= 32) pos_y = 0;
            }
            _V[0xF] = turned_off;
            break;
        }
        case 0xE:
            switch (opcode & 0xFF) {
                case 0x9E:
                    if (_input.keys[x]) _pc += 2;
                    break;
                case 0xA1:
                    if (!_input.keys[x]) _pc += 2;
                    break;
            }
            break;
        case 0xF:
            switch (opcode & 0xFF) {
                case 0x07:
                    _V[x] = _delay_timer;
                    break;
                case 0x0A: {
                    bool key_pressed = false;
                    for (int i = 0; i < 16; i++) {
                        if (_input.keys[i] != 0) {
                            _V[x] = i;
                            key_pressed = true;
                        }
                    }
                    if (!key_pressed) {
                        _pc -= 2;
                        return;
                    }
                    break;
                }
                case 0x15:
                    _delay_timer = _V[x];
                    break;
                case 0x18:
                    _sound_timer = _V[x];
                    break;
                case 0x1E:
                    _I += _V[x];
                    break;
                case 0x29:
                    _I = _V[x] * 5;
                    break;
                case 0x33: {
                    _mmu.writeByte(_I, _V[x] / 100);
                    _mmu.writeByte(_I + 1, (_V[x] / 10) % 10);
                    _mmu.writeByte(_I + 2, _V[x] % 10);
                    break;
                }
                case 0x55:
                    for (int i = 0; i <= x; i++) {
                        _mmu.writeByte(_I + i, _V[i]);
                    }
                    break;
                case 0x65:
                    for (int i = 0; i <= x; i++) {
                        _V[i] = _mmu.readByte(_I + i);
                    }
                    break;
            }
            break;
    }
}