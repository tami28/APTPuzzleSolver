//
// Created by Yoav on 08-Nov-17.
//

#include "PuzzlePiece.h"


PuzzlePiece::PuzzlePiece(const int id, int edges[4])
        : id(id), edges(edges) {}

PuzzlePiece::PuzzlePiece(string inputFileLine) {
    int currDelimPos, prvDelimPos=0, argsCount = 0;
    string lineDelimiter = " ";
    int args[5], param;
    while ((currDelimPos = s.find(delimiter) != std::string::npos)){
        argsCount +=1;
        if (argsCount > 5) {
            printBadLineError(args[0], inputFileLine);
            return INVALID_ARGUMENT_ERR_CODE;
        }
        try{
            param = std::stoi(s.substr(prvDelimPos, currDelimPos));
            args[argsCount] = param;
            }
        catch (const std::invalid_argument& ia) {
            printBadLineError(args[0], inputFileLine); //TODO: what if ID itself is broken? (what should be printed..?)
            return INVALID_ARGUMENT_ERR_CODE;
        }
        if (argsCount == 1 && (param < 1 || param > numElements)) {//TODO: numElements needs to be a global var, initialized before ecalling this func.
                printBadLineError(param, inputFileLine);
                return INVALID_ARGUMENT_ERR_CODE;}
        else if (edgeParam != EdgeDirections.MALE &&
                 edgeParam != EdgeDirections.FEMALE &&
                 edgeParam != EdgeDirections.STRAIGHT) {
                    printBadLineError(param, inputFileLine);
                    return INVALID_ARGUMENT_ERR_CODE;}
        prvDelimPos = currDelimPos; //todo: +1?
    }
    //Call regular Piece c'tor on parsed arguments:
    PuzzlePiece::PuzzlePiece(int args[0], args+1);
}


void PuzzlePiece::printBadLineError(int id, string inputFileLine){
    cout  << "Puzzle ID <" << id << "has wrong data: <" << inputFileLine << endl;
    }
