/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "Solver.h"

string outFilePath;

int main(int argc, char** argv){
#if DEBUG
    printf("IN DEBUG MODE!!!!\n");
#endif

    if (argc <2){
        std::cout<<"Error, no input file. Run COMMAND inputFilePath or COMMAND inputFilePath outputFilePath for specific output file location";
    }
    // Construct Puzzle:
    std::string inFilePath = argv[1];
    if (argc < 3){
        outFilePath = DEFAULT_OUTPUT_FILE;
    } else{
        outFilePath = argv[2];
    }
    Puzzle puzzle = Puzzle(inFilePath);
    // Check for Errors before continuing to solve:
    if (ErrorList::getNumErrors() > 0) {
        ErrorList::getErrorList()->toFile();
        return 1;
    }
    // Try to solve:
    Solver solver = Solver(puzzle);
    solver.solve();
    if (ErrorList::getNumErrors() > 0) {
        ErrorList::getErrorList()->toFile();
    }

    ErrorList::close();

}


