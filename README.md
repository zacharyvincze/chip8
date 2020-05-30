# chip8
CHIP-8 interpreter in C++ using SDL2.

# How to Use

## Compilation
*Note: requires `libsdl2-dev` and [nlohmann/json](https://github.com/nlohmann/json/releases)*.

```
mkdir build
cd build
cmake ..
make
cd ..
```

This should create the `chip8` executable in the `build` directory.

## Running the Executable
```
build//chip8 <path_to_rom>
```
Be sure to run this executable in the same directory that `config.json` is located in.

## Configuration

Configuring options such as CPU frequency, colors, and enabling fullscreen can be done in the `config.json` file located at the root of the project.

# TODO:
- Sound
- Keyboard mapping in configuration file
- CPU speed that's independent of the refresh rate
- There appears to be something wrong with the way keyboard inputs are being handled, this will be looked into at a later date