cmake_minimum_required(VERSION 3.7)
project(chip8)
set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC
    "src/*.cpp"
)

find_package(SDL2 REQUIRED)
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})