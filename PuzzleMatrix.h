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
    CellArray2D matrix;

    std::set<pair<int,int>> frontierCells; //TODO: can change to unorderd_set


public:
    PuzzleMatrix(int row, int col);
    Constraints operator()(int row, int col, Edge edge);

    void UpdateConstraintsOfNeighbour(PuzzlePiece* piece,Edge pieceEdgeToUpdateBy, Edge neighbourEdgeToUpdate,
                                                    int neighbourRow, int neighbourCol);
    void assignPieceToCell(PuzzlePiece* piece, int row, int col);
    bool isFit(PuzzlePiece* piece, int row, int col);
    void updateFrontiers(int row, int col);

    //TODO: destructor:
    //~PuzzleMatrix();
};

