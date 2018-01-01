/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Solver.h"
#include <chrono>
#include "const.h"

#define NO_INPUT_FILE_ERR_MSG "Error, no input file. Run COMMAND inputFilePath or COMMAND inputFilePath outputFilePath for specific output file location"
#define FLAG_USED_ONCE_ERR_MSG "Each flag should be used once."
#define WRONG_FLAG_USAGE_ERR_MSG "Wrong usage: should call flag -thread <d> with <d> number of wanted threads"
#define TOO_MANY_ARGS_ERR_MSG "Too many args, unused parameter: "
#define ROTATE_FLAG "-rotate"
#define THREADS_FLAG "-threads"

std::string outFilePath;
bool withRotations = false;

bool isCharPntInt(char* argv){
    for(char* c = argv; *c !='\0'; c++ ){
        if(!(*c >='0' && *c <='9')){
            return false;
        }
    }
    return true;
}


int main(int argc, char** argv){
    bool hasInputFile = false, hasOutputFile = false, hasThreads = false;
    std::string inFilePath;
    outFilePath = DEFAULT_OUTPUT_FILE;
    int numThreads = DEFAULT_NUM_THREADS;
    if (argc <2){
        std::cout<<NO_INPUT_FILE_ERR_MSG;
        return 1;
    }
    for(int i =1; i<argc; i++){
        if ((strcmp(argv[i], ROTATE_FLAG) == 0)){
            if (withRotations){
                std::cout<<FLAG_USED_ONCE_ERR_MSG<<std::endl;
                return 1;
            }
            withRotations = true;
            continue;
        }
        if ((strcmp(argv[i], THREADS_FLAG) == 0)){
            if (hasThreads){
                std::cout<<FLAG_USED_ONCE_ERR_MSG<<std::endl;
                return 1;
            }
            hasThreads = true;
            i++;
            if (i >= argc || !(isCharPntInt(argv[i]))){
                std::cout<<WRONG_FLAG_USAGE_ERR_MSG<<std::endl;
                return 1;
            }
            numThreads = atoi(argv[i]);
            continue;
        }
        if (!hasInputFile){
            inFilePath = argv[i];
            hasInputFile = true;
            continue;
        }
        if(!hasOutputFile){
            outFilePath = argv[i];
            hasOutputFile = true;
            continue;
        }
        std::cout <<TOO_MANY_ARGS_ERR_MSG<< argv[i] << std::endl;
        return 1;
    }
    // Try to solve:
    Solver solver(inFilePath, numThreads);
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


