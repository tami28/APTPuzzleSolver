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
    size_t currDelimPos, prvDelimPos=0;
    int argsCount = 0;
    string lineDelimiter = " ";
    int args[5], param;
    while ((currDelimPos = inputFileLine.find(lineDelimiter)) != string::npos){
        argsCount +=1;
        if (argsCount > 5) {
            //TODO:  handle error here
            id = -1;
        }
        param = stoi();
        param = atoi(inputFileLine.substr(prvDelimPos, currDelimPos));
        //TODO: need to check for exception in atoi, or use some other conversion...
        args[argsCount] = param;
        if (argsCount == 1 && (param < 1 || param > numPieces)) {//TODO: numElements needs to be a global var, initialized before ecalling this func.
                //TODO: handle bad piece-index err
        }
        else if (param != 1 &&
                 param != -1 &&
                 param != 0) { //TODO: change back to relevant enum values
            //todo: handle bad piece error
        }
        prvDelimPos = currDelimPos; //todo: +1?
    }
    //Call regular Piece c'tor on parsed arguments:
    *this = PuzzlePiece(args[0], args+1);
}
