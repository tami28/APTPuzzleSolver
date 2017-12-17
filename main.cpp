/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "RotatePuzzle.h"
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

    bool hasInputFile = false;
    std::string inFilePath;
    outFilePath = DEFAULT_OUTPUT_FILE;

    if (argc <2){
        std::cout<<"Error, no input file. Run COMMAND inputFilePath or COMMAND inputFilePath outputFilePath for specific output file location";
        return 1;
    }
    // Construct Puzzle:
    if ((strcmp(argv[1], "-rotate") == 0)){
        withRotations = true;
    } else{
        inFilePath = argv[1];
        hasInputFile = true;
    }

    if (argc < 3){
        if (!hasInputFile){
            std::cout<<"Error, no input file. Run COMMAND inputFilePath or COMMAND inputFilePath outputFilePath for specific output file location";
            return 1;
        }
        outFilePath = DEFAULT_OUTPUT_FILE;
    } else{
        if ((strcmp(argv[2], "-rotate") == 0)){
            withRotations = true;
        } else{
            outFilePath = argv[2];
        }
    }
    if (argc == 4){
        if ((strcmp(argv[3], "-rotate") == 0)){
            withRotations = true;
        } else{
            outFilePath = argv[3];
        }
    }


    // Try to solve:
    Solver solver = Solver(inFilePath);
    // Check for Errors before continuing to solve:
    if (ErrorList::getNumErrors() > 0) {
        ErrorList::getErrorList()->toFile();
        return 1;
    }
    solver.solve();
    if (ErrorList::getNumErrors() > 0) {
        ErrorList::getErrorList()->toFile();
    }

    ErrorList::close();

}


