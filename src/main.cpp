#include <SDL2/SDL.h>

#include "chip8.h"
#include "easylogging++.h"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    Chip8 chip8(argv[1]);
    return 0;
}