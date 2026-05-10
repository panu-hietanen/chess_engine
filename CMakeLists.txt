cmake_minimum_required(VERSION 3.20)
project(chess_engine C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(chess_engine
    src/main.c
    src/board.c
    src/utils.c
    src/piece.c
    src/moves.c
)

target_include_directories(chess_engine PRIVATE src)

if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(chess_engine PRIVATE /fsanitize=address)
elseif(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(chess_engine PRIVATE -fsanitize=address)
    target_link_options(chess_engine PUBLIC -fsanitize=address)
endif()

if (UNIX)
    target_link_libraries(chess_engine PUBLIC m)
endif()
