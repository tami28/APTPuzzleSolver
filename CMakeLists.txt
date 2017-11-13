cmake_minimum_required(VERSION 3.8)
project(ATPPUzzleSolver)

set(CMAKE_C_STANDARD 99)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # enable C++11 standard
set(SOURCE_FILES PuzzlePiece.cpp Exceptions.cpp Puzzle.cpp main.cpp)
set(HEADER_FILES PuzzlePiece.h Exceptions.h Puzzle.h const.h solver.h)
add_executable(main ${SOURCE_FILES})