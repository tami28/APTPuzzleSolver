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

#define ALTERNATIVE_ZERO_STRING "8"
#define ALTERNATIVE_ZERO_INT 8
#define DEFAULT_PIECE_ID -1
#define DEFAULT_EDGE Constraints::NONE


using namespace std;

class PuzzlePiece {
    int id;
    Constraints edges[4];

public:
    PuzzlePiece(); // Empty c'tor
    PuzzlePiece(int id, int edges[4]);//PuzzlePiece(const PuzzlePiece& otherPiece); //copy c'tor
    PuzzlePiece(const std::string inputFileLine); //c'tor from file-line

    int getId();
    Constraints getEdge(Edge edge); //TODO: return int* ??...
    bool canConnect(PuzzlePiece other, Edge edge); //can connect with other on given edge
};


#endif //PROJECT_PUZZLEPIECE_H
