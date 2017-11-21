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
    CellArray2D matrix; // TODO: make private again
    PuzzleMatrix(int row, int col);
    PuzzleMatrix(const PuzzleMatrix &other);
    Constraints operator()(int row, int col, Edge edge);

    void updateRequiredCounters(PuzzlePiece* piece, int row, int col);
    void UpdateConstraintsOfNeighbour(PuzzlePiece* piece,Edge pieceEdgeToUpdateBy, Edge neighbourEdgeToUpdate,
                                                    int neighbourRow, int neighbourCol);
    void assignPieceToCell(PuzzlePiece* piece, int row, int col);
    bool isFit(PuzzlePiece* piece, int row, int col);
    void updateFrontiers(int row, int col);
    std::set<pair<int,int>>::iterator getFrontierCellsIterator() const;
    const int getNrows() const;
    const int getNcols() const;

    std::set<pair<int,int>> frontierCells; //TODO: can change to unorderd_set. needs to be private with appropriate getters..

    void toFile(string path);
    string toString();
    string encode();
    void print();


    std::map<Constraints , int> requiredCounters;


    //TODO: destructor:
    //~PuzzleMatrix();
};

