//
// Created by Yoav on 08-Nov-17.
//

#ifndef PROJECT_PUZZLEPIECE_H
#define PROJECT_PUZZLEPIECE_H

#include <string>
#include<cstdlib>
using namespace std;

//enum class Edge {RIGHT, LEFT, UP, DOWN} ;
//enum class EdgeDirections {MALE = 1, FEMALE = -1, STRAIGHT = 0} ;


class PuzzlePiece {
    int id;
    int edges[4];
public:
    PuzzlePiece(const int id, int edges[4]);
    //PuzzlePiece(const PuzzlePiece& otherPiece); //copy c'tor
    PuzzlePiece(string inputFileLine); //create a piece from a line in the input file

    const int getId();
    int* getEdges(); //TODO: return int* ??...
    void setPosition(const int x, const int y);
    //bool canConnect(PuzzlePiece other, Edge edge); //can connect with other on given edge
};


#endif //PROJECT_PUZZLEPIECE_H
