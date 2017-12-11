cmake_minimum_required(VERSION 3.8)
project(ATPPUzzleSolver)
set(CMAKE_C_STANDARD 99)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") # enable C++11 standard
set(SOURCE_FILES main.cpp PuzzlePiece.cpp Exceptions.cpp Puzzle.cpp PuzzleMatrix.cpp Solver.cpp ConstraintsTable.cpp RotateSolver.cpp RotateSolver.h RotatePuzzle.cpp RotatePuzzle.h SolvabilityVerifier.cpp)
set(HEADER_FILES PuzzlePiece.h Exceptions.h Puzzle.h const.h Solver.h PuzzleMatrix.h ConstraintsTable.h SolvabilityVerifier.h)
add_executable(APTPuzzleSolver ${SOURCE_FILES})
if (CMAKE_BUILD_TYPE MATCHES Debug)
add_definitions(-DDEBUG=1)
endif()

