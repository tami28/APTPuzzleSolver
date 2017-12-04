//
// Created by Yoav on 13-Nov-17.
//

#pragma once

#ifndef ATPPUZZLESOLVER_PUZZLWMATRIX_H
#define ATPPUZZLESOLVER_PUZZLWMATRIX_H

#endif //ATPPUZZLESOLVER_PUZZLWMATRIX_H

#include "const.h"
#include "PuzzlePiece.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class PuzzleMatrix{

    int nrows;
    int ncols;
    typedef std::vector<Constraints> ConstraintsArray;

    typedef struct Cell{
        PuzzlePiece* piece = NULL;
        ConstraintsArray constraints = ConstraintsArray(4, Constraints::NONE);
    } Cell;
    typedef std::vector<Cell> CellArray;
    typedef std::vector<CellArray> CellArray2D;

public:
    CellArray2D matrix;
    PuzzleMatrix(int row, int col);
    PuzzleMatrix(const PuzzleMatrix &other);
    Constraints operator()(int row, int col, Edge edge);

    void updateRequiredCounters(PuzzlePiece* piece, int row, int col);
    void UpdateConstraintsOfNeighbour(PuzzlePiece* piece,Edge pieceEdgeToUpdateBy, Edge neighbourEdgeToUpdate,
                                                    int neighbourRow, int neighbourCol);
    void assignPieceToCell(PuzzlePiece* piece, int row, int col);
    bool isFit(PuzzlePiece* piece, int row, int col);
    int getNrows() const;
    int getNcols() const;

    void toFile(string path);
    string toString();
    void print();

    std::map<Constraints , int> requiredCounters;

    void constraintsOfCell(int i,int j, int*);
};

