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
        Rotate rotation;
    } Cell;
    typedef std::vector<Cell> CellArray;
    typedef std::vector<CellArray> CellArray2D;
    std::map<Constraints , int> _requiredCounters; // TODO: move to priv and write getter
    std::map<outerFrameConstraints, int> _requieredFrameConstraints; //number of outer-frame straight edges needed for this puzzleMatrix. //todo move to private


public:
    CellArray2D matrix;
    PuzzleMatrix(int row, int col);
    PuzzleMatrix(const PuzzleMatrix &other);
    Constraints operator()(int row, int col, Edge edge);

    void updateRequiredCounters(PuzzlePiece* piece, Rotate rotation, int row, int col);
    void UpdateConstraintsOfNeighbour(PuzzlePiece* piece, Rotate rotation, Edge pieceEdgeToUpdateBy, Edge neighbourEdgeToUpdate,
                                                    int neighbourRow, int neighbourCol);
    void assignPieceToCell(PuzzlePiece* piece,Rotate rotation, int row, int col);
    bool isFit(PuzzlePiece* piece, int row, int col);
    int getNrows() const;
    int getNcols() const;

    void toFile(string path);
    string toString();
    void print();

    std::map<Constraints , int> getRequiredCounters();
    void setRequiredCounters(std::map<Constraints , int> requiredCounters);
    std::map<outerFrameConstraints, int> getRequiredFrameConstraints();
    void setRequiredFrameConstraints(std::map<outerFrameConstraints , int> requiredFrameCounters);
    void updateRequiredFrameCounters(int row, int col);
    int getOuterFrameRequirementAt(outerFrameConstraints c);
    bool isCornerRequired(Corners c);
    void constraintsOfCell(int row,int col, int*);
    void getConstraintsOfCell(int row, int col, int* res);

};

