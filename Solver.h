/*
 * Solver.h
 *
 *  Created on: 13 nov 2017
 *      Author: Tami
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "Puzzle.h"
#include "RotatePuzzle.h"
#include "const.h"
#include "PuzzleMatrix.h"
#include "Exceptions.h"
#include "SolvabilityVerifier.h"
#include <memory>
#include <numeric>

//TODO: decide about these constants here (0.5? 0.3?)
#define MIN_NUM_PIECES_TO_CHECK_SUFFICIENT_CONSTRAINTS 0 //todo chane to 30
#define PIECES_RATIO_TO_CHECK_SUFFICIENT_CONSTRAINTS 0.5

/*
 * Used to determine in what order to go over the puzzle.
 */
class Step{
    friend class Solver;
    friend class RotateSolver;
protected:
    int i = 0;
    int j =0 ;
    int nrow = 0;
    int ncol = 0;
public:
    /*
     * update i&j to the next cell to go over. If we are at the end of the puzzle, meaning no next step - return false.
     */
    virtual bool nextStep();
    /*
     * update i&j to the previous cell we wentover. If we are at the start of the puzzle, meaning no prev step s- return false.
     */
    virtual bool prevStep();
    Step(){};
    Step(int n, int m) : nrow(n), ncol(m){};
};

/*
 * Used to go over frame, then by row inside.
 */
class StepFrame :public Step{
    friend class Solver;
    friend class RotateSolver;
public:
    bool nextStep();
    bool prevStep();
    StepFrame(int n, int m) : Step(n,m){};

};


class Solver{
private:
    std::unique_ptr<Puzzle> _puzzle;
    std::vector<int> indices;
    std::unique_ptr<Step> next;
    bool isFrame = false;

public:
    Solver() = default ;
    Solver(string fileName){
        if (withRotations){
            _puzzle = std::make_unique<RotatePuzzle>(fileName);
        }else {
            _puzzle = std::make_unique<Puzzle>(fileName);
        }

    };
    //void setPuzzle(Puzzle& p);
    virtual std::vector<pair<int, int>> getPossiblePuzzleSizes();

    void solve();

    bool piecefitsConstrains(PuzzlePiece& piece, char constraints[4]);
    virtual bool _solveForSize(PuzzleMatrix& pm, vector<int> indices);
    //virtual bool _solveForSize(PuzzleMatrix& pm, unordered_set<int> usedIDs, PuzzleMatrix *result, int row, int col);
    virtual bool hasSingleRowColSolution();
    virtual bool _isFitForCell(int i, std::unordered_set<int>& badPieces,  vector<int> usedIDs, Rotate rotation);
    virtual bool solverFinished(PuzzleMatrix& pm,vector<int> usedIDs);
    int COUNT; //TODO; rem
};

//solution table
//Solution finder gets puzzle & solution table and return a table of pieces representing the solution
//Solver will have function that goes over possible legit sizes and call SolutionFinder

#endif /* SOLVER_H_ */
