cmake_minimum_required(VERSION 3.8)
project(ATPPUzzleSolver)

set(SOURCE_FILES PuzzlePiece.cpp  main.cpp Puzzle.cpp Exceptions.cpp)
set(HEADER_FILES PuzzlePiece.h  Puzzle.h const.h Exceptions.h Solver.h)
add_executable(APTPuzzleSolver ${SOURCE_FILES})
