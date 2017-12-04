/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "Solver.h"
#include <chrono>
#include "const.h"
typedef std::chrono::high_resolution_clock Clock;
string outFilePath;
bool withRotations = false;

int main(int argc, char** argv){
#if DEBUG
    printf("IN DEBUG MODE!!!!\n");
#endif

    ConstraintsTable tab = ConstraintsTable();


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
    if (argc == 4){
        withRotations = (strcmp(argv[3], "-rotate") == 0); //TODO: fix so -rotate can be anywhere in cmnd line..
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


