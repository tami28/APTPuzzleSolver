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

    bool _solveForSize(PuzzleMatrix& pm, vector<int> indices, PuzzleMatrix* result); //TODO: move back to private
    void solve();
    int COUNTER = 0; //todo rm
    //TODO: destructors
};

//solution table
//Solution finder gets puzzle & solution table and return a table of pieces representing the solution
//Solver will have function that goes over possible legit sizes and call SolutionFinder

#endif /* SOLVER_H_ */
