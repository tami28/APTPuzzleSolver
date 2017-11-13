//
// Created by Yoav on 08-Nov-17.
//

#ifndef PROJECT_PUZZLEPIECE_H
#define PROJECT_PUZZLEPIECE_H

#include <string>
#include <cstring>
#include<cstdlib>
#include "const.h"

#define ALTERNATIVE_ZERO_STRING "8"
#define ALTERNATIVE_ZERO_INT 8
#define DEFAULT_PIECE_ID -1
#define DEFAULT_EDGE -1


using namespace std;

class PuzzlePiece {
    int id;
    int edges[4];
public:
    PuzzlePiece(); // Empty c'tor
    PuzzlePiece(int id, int edges[4]);//PuzzlePiece(const PuzzlePiece& otherPiece); //copy c'tor
    PuzzlePiece(string inputFileLine); //c'tor from file-line

    int getId();
    //int* getEdges(); //TODO: return int* ??...
    //void setPosition(const int x, const int y);
    bool canConnect(PuzzlePiece other, Edge edge); //can connect with other on given edge
};


#endif //PROJECT_PUZZLEPIECE_H
