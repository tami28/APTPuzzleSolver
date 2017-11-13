cmake_minimum_required(VERSION 3.8)
project(ATPPUzzleSolver)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES PuzzlePiece.cpp  Exceptions.cpp Puzzle.cpp main.cpp)
set(HEADER_FILES PuzzlePiece.h Exceptions.h Puzzle.h const.h)
add_executable(main ${SOURCE_FILES})