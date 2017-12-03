//
// Created by Tami on 30/11/2017.
//
#include "Solver.h"

#ifndef ATPPUZZLESOLVER_ROTATESOLVER_H
#define ATPPUZZLESOLVER_ROTATESOLVER_H


class RotateSolver : public Solver{
public:
    bool _solveForSize(PuzzleMatrix& pm, vector<int> indices, PuzzleMatrix *result, int row, int col);
    bool _isFitForCell(int i, std::unordered_set<string>& badPieces, char consts[]); //TODO: rotate accorddingly!
    //TODO: getPossiblePuzzleSizes that returns only sizes where i<=j!
};


#endif //ATPPUZZLESOLVER_ROTATESOLVER_H
