//
// Created by Yoav on 08-Nov-17.
//

#include "PuzzlePiece.h"
#include "const.h"


/*
 * empy constructor for a Piece object
 */
PuzzlePiece::PuzzlePiece(): id(DEFAULT_PIECE_ID), edges{DEFAULT_EDGE,DEFAULT_EDGE,DEFAULT_EDGE,DEFAULT_EDGE}{}

/*
 * Cinstructor for a Piece object from id + edges array
 */
PuzzlePiece::PuzzlePiece(int id, int inputEdges[4])
        : id(id) {
    for (int i=0; i<4; i++){
        edges[i] = inputEdges[i];
    }
}


/*
 * constructor for a Piece object from a line in the text file.
 */
PuzzlePiece::PuzzlePiece(string inputFileLine) {
    size_t currDelimPos, prvDelimPos=0;
    int argsCount = 0;
    string lineDelimiter = " ";
    int args[5], param;
    while ((currDelimPos = prvDelimPos + inputFileLine.find(lineDelimiter)) != string::npos){
        argsCount +=1;
        if (argsCount > 5) {
            //TODO:  handle error here
            id = -1;
            break;
        }
        char* paramstr =(char*) inputFileLine.substr(prvDelimPos, currDelimPos).c_str();
        // Use of ALTERNATIVE_ZERO_STRING / ALTERNATIVE_ZERO_INT is to overcome atoi's error value being 0.
        if (strcmp(paramstr,"0") == 0){
            paramstr = (char *) ALTERNATIVE_ZERO_STRING;
        }
        param = atoi(paramstr);
        if (param == 0){}//TODO: handle atoi exception (bad line)
        if (param == ALTERNATIVE_ZERO_INT) {param = 0;}

        args[argsCount-1] = param;
        if (argsCount == 1 && (param < 1 || param > numPieces)) {
                //TODO: handle bad piece-index err
        }
        else if (param != Constraints::MALE &&
                 param != Constraints::FEMALE &&
                 param != Constraints::STRAIGHT) {
            //todo: handle bad piece error
        }
        prvDelimPos = currDelimPos+1;
    }
    //Call regular Piece c'tor on parsed arguments:
    *this = PuzzlePiece(args[0], args+1);
}


int PuzzlePiece::getId(){
    return id;
}

