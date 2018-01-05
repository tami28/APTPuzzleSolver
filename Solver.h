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
#include "Steper.h"
#include <thread>
#include <mutex>

#define MIN_NUM_PIECES_TO_CHECK_SUFFICIENT_CONSTRAINTS 30
#define PIECES_RATIO_TO_CHECK_SUFFICIENT_CONSTRAINTS 0.5
#define STRAIGHT_EDGES_RATIO_FOR_FRAME_STEPS 0.25

/*
 * Used to determine in what order to go over the puzzle.
 */


class Solver{

private:
    std::unique_ptr<Puzzle> _puzzle;
    std::vector<int> indices;
    std::map<int,std::unique_ptr<StepRow>> steppersMap;
    bool isFrame = false;
    void setStep(int nrow, int ncol, int threadIndex);
    PuzzleMatrix _solution;
    bool _solved;
    unsigned int _numThreads;
    void threadSolveForSize(vector<pair<int,int>> sizes, int threadIndex);
    virtual bool solverFinished(vector<int> usedIDs, int threadIndex);
    vector<vector<pair<int,int>>> divideSizesToThreads(vector<pair<int,int>> allPossibleSizes);
    std::mutex _declaringSolvedMutex;
    int performValidityChecks();

public:
    Solver() = default ;
    Solver(string fileName, unsigned int numThreads){
        _solved = false;
        _numThreads = numThreads;
        if (Puzzle::withRotations){
            _puzzle = std::make_unique<RotatePuzzle>(fileName);
        }else {
            _puzzle = std::make_unique<Puzzle>(fileName);
        }

    };
    //void setPuzzle(Puzzle& p);
    virtual std::vector<pair<int, int>> getPossiblePuzzleSizes();

    void solve();

    bool piecefitsConstrains(PuzzlePiece& piece, char constraints[4]);
    virtual bool _solveForSize(PuzzleMatrix& pm, vector<int> indices, int threadIndex);
    virtual bool hasSingleRowColSolution();
    virtual bool _isFitForCell(int i, std::unordered_set<int>& badPieces,  vector<int> usedIDs, Rotate rotation);

};

#endif /* SOLVER_H_ */
