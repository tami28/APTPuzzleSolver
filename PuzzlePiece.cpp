//
// Created by Yoav on 08-Nov-17.
//

#include "PuzzlePiece.h"


PuzzlePiece::PuzzlePiece(const int id, int edges[4])
        : id(id), edges(edges) {}

PuzzlePiece::PuzzlePiece(string inputFileLine) {
    // todo: extract id and edges from string, and call previos c'tor on threm
}