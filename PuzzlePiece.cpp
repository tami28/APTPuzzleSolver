//
// Created by Yoav on 08-Nov-17.
//

#include "PuzzlePiece.h"
#include "const.h"

PuzzlePiece::PuzzlePiece(int id, int inputEdges[4])
        : id(id) {
    for (int i=0; i<4; i++){
        edges[i] = inputEdges[i];
    }
}

PuzzlePiece::PuzzlePiece(string inputFileLine) {
    int currDelimPos, prvDelimPos=0, argsCount = 0;
    string lineDelimiter = " ";
    int args[5], param;
    while ((currDelimPos = inputFileLine.find(lineDelimiter) != std::string::npos)){
        argsCount +=1;
        if (argsCount > 5) {
            //TODO:  handle error here
            return NULL;
        }

        if (param = atoi(inputFileLine.substr(prvDelimPos, currDelimPos)) == 0){
            //todo: could not convert string to int, handle err
        }
        args[argsCount] = param;

        if (argsCount == 1 && (param < 1 || param > numPieces)) {//TODO: numElements needs to be a global var, initialized before ecalling this func.
                //TODO: handle bad piece-index err
        }
        else if (edgeParam != EdgeDirections.MALE &&
                 edgeParam != EdgeDirections.FEMALE &&
                 edgeParam != EdgeDirections.STRAIGHT) {
                    //todo: handle bad piece error
        }
        prvDelimPos = currDelimPos; //todo: +1?
    }
    //Call regular Piece c'tor on parsed arguments:
    PuzzlePiece::PuzzlePiece(args[0], args+1);
}


void PuzzlePiece::printBadLineError(int id, string inputFileLine){
    cout  << "Puzzle ID <" << id << "has wrong data: <" << inputFileLine << endl;
    }
