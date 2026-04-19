cmake_minimum_required(VERSION 3.20)
project(chess_engine C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(chess_engine
    src/main.c
    src/board.c 
    src/utils.c 
    src/piece.c
    src/render.c
    src/ui.c
    src/moves.c
)

target_include_directories(chess_engine PRIVATE src)

if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(chess_engine PRIVATE /fsanitize=address)
endif()

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
)
FetchContent_MakeAvailable(raylib)

target_link_libraries(chess_engine PRIVATE raylib)