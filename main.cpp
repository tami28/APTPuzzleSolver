/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Solver.h"
#include <chrono>
#include "const.h"

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
        std::cout<<"Error, no input file. Run COMMAND inputFilePath or COMMAND inputFilePath outputFilePath for specific output file location";
        return 1;
    }
    for(int i =1; i<argc; i++){
        if ((strcmp(argv[i], "-rotate") == 0)){
            if (withRotations){
                std::cout<<"Each flag should be used once."<<std::endl;
                return 1;
            }
            withRotations = true;
            continue;
        }
        if ((strcmp(argv[i], "-threads") == 0)){
            if (hasThreads){
                std::cout<<"Each flag should be used once."<<std::endl;
                return 1;
            }
            hasThreads = true;
            i++;
            if (i >= argc || !(isCharPntInt(argv[i]))){
                std::cout<<"Wrong usage: should call flag -thread <d> with <d> number of wanted threads"<<std::endl;
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
        std::cout <<"To many args, unused parameter: "<< argv[i] << std::endl;
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


