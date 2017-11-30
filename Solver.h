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
    std::vector<int> indices;
    unordered_set<string> memoizationSet;
    bool checkSufficientConstraints(vector<int> indices, PuzzleMatrix *pm);

public:
    Solver();
    Solver(Puzzle& p);
    //void setPuzzle(Puzzle& p);
    std::vector<pair<int, int>> getPossiblePuzzleSizes();

    void solve();

    bool piecefitsConstrains(PuzzlePiece& piece, char constraints[4]);
    //bool _solveForSize(PuzzleMatrix& pm, vector<int> indices, PuzzleMatrix *result, int row, int col);
    bool _solveForSize(PuzzleMatrix& pm, vector<int> usedIDs, PuzzleMatrix *result, int row, int col);
    bool hasSingleRowColSolution();
    int COUNT; //TODO; rem
};

//solution table
//Solution finder gets puzzle & solution table and return a table of pieces representing the solution
//Solver will have function that goes over possible legit sizes and call SolutionFinder

#endif /* SOLVER_H_ */
