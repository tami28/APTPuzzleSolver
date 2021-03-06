//
// Created by Yoav on 08-Nov-17.
//

#include "PuzzlePiece.h"


/*
 * empy constructor for a Piece object
 */

PuzzlePiece::PuzzlePiece(): edges{DEFAULT_EDGE,DEFAULT_EDGE,DEFAULT_EDGE,DEFAULT_EDGE}{}

/*
 * Constructor for a Piece object from id + edges array.
 * This c'tor assumes valid input in inputEdges.
 */
PuzzlePiece::PuzzlePiece(int id, int inputEdges[4])
        : id(id){
    this->encoding = 0;
    for (int i=0; i<4; i++){
        edges[i] = (Constraints) inputEdges[i];
        this->encoding +=  (inputEdges[i]+1)*pow(3,i); //encode the shape of the piece as a base-3 number (after increasing value of each edge by 1)

    }

}


/*
 * constructor for a Piece object from a line in the text file.
 */
PuzzlePiece::PuzzlePiece(const std::string& inputFileLine, vector<int> &idsFromFile) {
    size_t currDelimPos, prvDelimPos=0;
    int argsCount = 0, piece_id, args[5], param;
    string lineDelimiter = " ";
    bool endOfLine = false, encounteredError = false;
    int numNumericParams = 0;

    vector<Error> errVec;
    while (!endOfLine){
        if ((currDelimPos = inputFileLine.substr(prvDelimPos).find(lineDelimiter)) == string::npos){
            endOfLine = true;
        }
        currDelimPos += prvDelimPos;
        argsCount +=1;
        if (argsCount > 5) {
            //More than 5 numbers in line --> invalid piece representation.
            encounteredError = true;
            errVec.push_back(Error(WRONG_PIECE_FORMAT, inputFileLine));
            //throw Error(WRONG_PIECE_FORMAT, inputFileLine);
        }
//        std::unique_ptr<char[]> paramstr = std::make_unique<char[]>(inputFileLine.substr(prvDelimPos, currDelimPos-prvDelimPos).length() + 1);
//        std::strcpy(paramstr.get(),inputFileLine.substr(prvDelimPos, currDelimPos-prvDelimPos).c_str() );
        string paramstr = inputFileLine.substr(prvDelimPos, currDelimPos-prvDelimPos);
        // Use of ALTERNATIVE_ZERO_STRING / ALTERNATIVE_ZERO_INT is to overcome atoi's encounteredError value being 0.
        if (paramstr.compare("0")== 0){
            paramstr = ALTERNATIVE_ZERO_STRING;
        }
        param = atoi(paramstr.c_str());

        if (param == 0 && argsCount == 1){
            //the ID given in the input line is not an int --> Error 1.5 in exercise updates:
            encounteredError = true;
            errVec.push_back(Error(WRONG_PIECE_FORMAT, inputFileLine, piece_id));
            throw errVec; //Throwing here because of correction 1.7 in excersize corrections.
            //throw Error(WRONG_PIECE_FORMAT, inputFileLine, piece_id);
        }

        if (argsCount == 1) {
            piece_id = param;
        }
        if (param == 0 && argsCount > 1){
            if (!encounteredError){
                encounteredError = true;
                errVec.push_back(Error(WRONG_PIECE_FORMAT, inputFileLine, piece_id));
            }
            //One of the E-d-g-e-s given in the input line is not an int --> invalid piece representation.
            //throw Error(WRONG_PIECE_FORMAT, inputFileLine, piece_id);
        }
        numNumericParams++; // Got here -> the parameter we are parsing now is an integer.
        if (param == ALTERNATIVE_ZERO_INT) {param = 0;}
        args[argsCount-1] = param;
        if (argsCount == 1 && (param < 1 || (unsigned int) param > Environment::getNumPieces())) {
            //Input ID is numeric but not in the valid range:
            encounteredError = true;
            errVec.push_back(Error(_WRONG_PIECE_ID, param));
            throw errVec; //Throwing here because of correction 1.7 in excersize corrections.
            //throw Error(_WRONG_PIECE_ID, param);
        }
        else if (argsCount != 1 &&
                (param != Constraints::MALE &&
                 param != Constraints::FEMALE &&
                 param != Constraints::STRAIGHT)) {
            encounteredError = true;
            errVec.push_back(Error(WRONG_PIECE_FORMAT, inputFileLine, piece_id));
            //throw Error(WRONG_PIECE_FORMAT, inputFileLine, piece_id);
        }
        prvDelimPos = currDelimPos+1;
    }

    //If all parsed parameters are numeric:
    if (numNumericParams == 5) {
        idsFromFile.push_back(piece_id);
    }
    // Report errors:
    if (encounteredError) { throw errVec; }
    //Call regular Piece c'tor on parsed arguments:
    *this = PuzzlePiece(args[0], args+1);
}



int PuzzlePiece::getId(){
    return id;
}

/*
 * Given another piece, return true if they can legally connect on a given edge (LEFT, TOP, RIGHT or BOTTOM), false o/w.
 */
bool PuzzlePiece::canConnect(PuzzlePiece& other, Edge edge){
    switch (this->edges[edge]){
        case Constraints::STRAIGHT:
            return other.edges[edge] == Constraints::STRAIGHT;
        case Constraints::MALE:
            return other.edges[edge] == Constraints::FEMALE;
        case Constraints::FEMALE:
            return other.edges[edge] == Constraints::MALE;
        case Constraints::NONE:
            return true;
    }
    return false;
}


Constraints PuzzlePiece::getConstraint(Edge edge, Rotate rotation){
    int offset = ( (int)edge - (rotation /90) )%4;
    offset = offset >= 0 ? offset : 4 + offset; //note offset is negative in second case.
    return  this->edges[offset];
}

Constraints PuzzlePiece::getConstraint(Edge edge){
    return this->edges[edge];
}

Constraints PuzzlePiece::getOppositeConstraint(Edge edge){
    switch (this->edges[edge]){
        case MALE:
            return FEMALE;
        case FEMALE:
            return MALE;
        case STRAIGHT:
            return STRAIGHT;
        default:
            return NONE;
    }
}


int PuzzlePiece::getEncoding(){
    return this->encoding;
}


string PuzzlePiece::getConstraintStr(Rotate rotation){
    char c[4];
    for (int i = LEFT; i<= BOTTOM; i++){
        c[i] = this->getConstraint((Edge) i, rotation);
    }
    return string(c, 4);
}

int PuzzlePiece::getConstraintsKey(Rotate rotation){
    int key = 0;
    for (int i = LEFT; i<= BOTTOM; i++){
        key += (this->getConstraint((Edge) i, rotation) +1 )*pow(10,3-i);
    }
    return key;
}
