//
// Created by Yoav on 08-Nov-17.
//

#ifndef PROJECT_PUZZLEPIECE_H
#define PROJECT_PUZZLEPIECE_H

#include <string>
#include <cstring>
#include<cstdlib>
#include "const.h"
#include "Exceptions.h"

#define ALTERNATIVE_ZERO_STRING "-2"
#define ALTERNATIVE_ZERO_INT -2
#define DEFAULT_PIECE_ID -1
#define DEFAULT_EDGE Constraints::NONE


using namespace std;

class PuzzlePiece {
    int id;
    int encoding;
    Constraints edges[4];

public:
    PuzzlePiece(); // Empty c'tor
    PuzzlePiece(int id, int edges[4]);//PuzzlePiece(const PuzzlePiece& otherPiece); //copy c'tor
    PuzzlePiece(const std::string& inputFileLine); //c'tor from file-line

    int getId();
    int getEncoding();
    Constraints getConstraint(Edge edge);
    Constraints getOppositeConstraint(Edge edge);
    bool canConnect(PuzzlePiece& other, Edge edge); //can connect with other on given edge
    string getConstraintStr();
};


#endif //PROJECT_PUZZLEPIECE_H
