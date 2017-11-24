/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "Solver.h"


//TODO: add handling bad input params from cmnd line
string outFilePath;

int main(int argc, char** argv){
#if DEBUG
    printf("IN DEBUG MODE!!!!\n");
#endif

    // Construct Puzzle:
    std::string inFilePath = argv[1];
    outFilePath = argv[2];
    Puzzle puzzle = Puzzle(inFilePath);

    // Check for Errors before continuing to solve:
    if (ErrorList::getNumErrors() > 0) {
        ErrorList::getErrorList()->toFile();
    }
    // Try to solve:
    Solver solver = Solver(puzzle);
    solver.solve();

    if (ErrorList::getNumErrors() > 0) {
        ErrorList::getErrorList()->toFile();
    }


}

//TODO: close ErrorList


