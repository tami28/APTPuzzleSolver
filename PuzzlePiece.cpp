//
// Created by Yoav on 08-Nov-17.
//

#include "PuzzlePiece.h"


/*
 * empy constructor for a Piece object
 */
PuzzlePiece::PuzzlePiece(): id(DEFAULT_PIECE_ID), edges{DEFAULT_EDGE,DEFAULT_EDGE,DEFAULT_EDGE,DEFAULT_EDGE}{}

/*
 * Constructor for a Piece object from id + edges array.
 * This c'tor assumes valid input in inputEdges.
 */
PuzzlePiece::PuzzlePiece(int id, int inputEdges[4])
        : id(id) {
    for (int i=0; i<4; i++){
        edges[i] = (Constraints) inputEdges[i];
    }
}


/*
 * constructor for a Piece object from a line in the text file.
 */
PuzzlePiece::PuzzlePiece(string inputFileLine) {
    ErrorList* errList = ErrorList::getErrorList();
    size_t currDelimPos, prvDelimPos=0;
    int argsCount = 0;
    string lineDelimiter = " ";
    int args[5], param;
    while ((currDelimPos = prvDelimPos + inputFileLine.find(lineDelimiter)) != string::npos){
        argsCount +=1;
        if (argsCount > 5) {
            //More than 5 numbers in line --> invalid piece representation.
            (*ErrorList::getErrorList()).add(Error(WRONG_PIECE_FORMAT, inputFileLine));
            id = INVALID_PIECE_ID;
            break;
        }
        char* paramstr =(char*) inputFileLine.substr(prvDelimPos, currDelimPos).c_str();
        // Use of ALTERNATIVE_ZERO_STRING / ALTERNATIVE_ZERO_INT is to overcome atoi's error value being 0.
        if (strcmp(paramstr,"0") == 0){
            paramstr = (char *) ALTERNATIVE_ZERO_STRING;
        }
        param = atoi(paramstr);
        if (param == 0 && argsCount > 1){
            //One of the E-d-g-e-s given in the input line is not an int --> invalid piece representation.
            (*ErrorList::getErrorList()).add(Error(WRONG_PIECE_FORMAT, inputFileLine));
        }
        if (param == 0 && argsCount == 1){
            //the ID given in the input line is not an int --> Error 1.5 in exercise updates:
            (*errList).add(Error(PIECE_ID_NAN, param));
            //TODO: make sure this works according to section 1.7 in updates

        }
        if (param == ALTERNATIVE_ZERO_INT) {param = 0;}

        args[argsCount-1] = param;
        if (argsCount == 1 && (param < 1 || param > numPieces)) {
            //Input ID is numeric but not in the valid range:
            (*errList).add(Error(WRONG_PIECE_ID, param));
            //TODO: make sure this works according to section 1.7 in updates
        }
        else if (param != Constraints::MALE &&
                 param != Constraints::FEMALE &&
                 param != Constraints::STRAIGHT) {
            (*ErrorList::getErrorList()).add(Error(WRONG_PIECE_FORMAT, inputFileLine));
        }
        prvDelimPos = currDelimPos+1;
    }
    //Call regular Piece c'tor on parsed arguments:
    *this = PuzzlePiece(args[0], args+1);
}


int PuzzlePiece::getId(){
    return id;
}

/*
 * Given another piece, return true if they can legally connect on a given edge (LEFT, TOP, RIGHT or BOTTOM), false o/w.
 */
bool PuzzlePiece::canConnect(PuzzlePiece other, Edge edge){
    switch (this->edges[edge]){
        case Constraints::STRAIGHT:
            return other.edges[edge] == Constraints::STRAIGHT;
        case Constraints::MALE:
            return other.edges[edge] == Constraints::FEMALE;
        case Constraints::FEMALE:
            return other.edges[edge] == Constraints::MALE;
    }
    return false;
}


Constraints PuzzlePiece::getEdge(Edge edge){
    return this->edges[edge];
}
