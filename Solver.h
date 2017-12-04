/*
 * Solver.h
 *
 *  Created on: 13 nov 2017
 *      Author: Tami
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "Puzzle.h"
#include "const.h"
#include "PuzzleMatrix.h"
#include "Exceptions.h"

class Solver{
private:
    Puzzle _puzzle;
    //std::vector<int> indices;
    virtual bool checkSufficientConstraints(vector<int> indices, PuzzleMatrix *pm);

public:
    Solver();
    Solver(Puzzle& p);
    //void setPuzzle(Puzzle& p);
    virtual std::vector<pair<int, int>> getPossiblePuzzleSizes();

    void solve();

    bool piecefitsConstrains(PuzzlePiece& piece, char constraints[4]);
    virtual bool _solveForSize(PuzzleMatrix& pm, vector<int> indices, PuzzleMatrix *result, int row, int col);
    //virtual bool _solveForSize(PuzzleMatrix& pm, unordered_set<int> usedIDs, PuzzleMatrix *result, int row, int col);
    virtual bool hasSingleRowColSolution();
    virtual bool _isFitForCell(int i, std::unordered_set<string>& badPieces,  vector<int> usedIDs);
    virtual bool solverFinished(PuzzleMatrix& pm,vector<int> usedIDs, int row, int col);
    int COUNT; //TODO; rem
};

//solution table
//Solution finder gets puzzle & solution table and return a table of pieces representing the solution
//Solver will have function that goes over possible legit sizes and call SolutionFinder

#endif /* SOLVER_H_ */
