/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "Solver.h"
#include "const.h"
#include "Exceptions.h"
#include <time.h>
#include "PuzzleMatrix.h"


//TODO: add handling bad input params from cmnd line
string outFilePath;

int main(int argc, char** argv){
#if DEBUG
    printf("IN DEBUG MODE!!!!\n");
#endif
    std::string inFilePath = argv[1];
    outFilePath = argv[2];
    Puzzle puzzle = Puzzle(inFilePath);

    //check for Errors before continuing to solve:
    if (ErrorList::getNumErrors() > 0) {
        ErrorList::getErrorList()->toFile();
    }

    Solver solver = Solver(puzzle);
    clock_t t;
    t = clock();
    solver.solve();
    if (ErrorList::getNumErrors() > 0) {
        ErrorList::getErrorList()->toFile();
    }
    t = clock() - t;
    printf ("took  %d clicks (%f seconds).\n",t,((float)t)/CLOCKS_PER_SEC); // TODO rm
    printf("Done\n");  // TODO rm
}

//TODO: close ErrorList


