//
// Created by Yoav on 11-Dec-17.
//

#ifndef ATPPUZZLESOLVER_SOLVABILITYVERIFIER_H
#define ATPPUZZLESOLVER_SOLVABILITYVERIFIER_H

#include "const.h"
#include "PuzzleMatrix.h"
#include "Puzzle.h"
#include <memory>

class SolvabilityVerifier {
public:
    SolvabilityVerifier(std::unique_ptr<Puzzle>  &puzzle, PuzzleMatrix  &pm, vector<int>  &usedIDs) : _puzzle(puzzle), _pm(pm), _usedIDs(usedIDs){};
    bool verifySolvabilityConstraints();
private:
    std::unique_ptr<Puzzle> const &_puzzle;
    PuzzleMatrix & _pm;
    vector<int> & _usedIDs;
    std::unordered_map<int, int> _sumConstraints;
    std::map<outerFrameConstraints, int> _sumOuterFrameEdges;
    bool _TL_corner, _BL_corner, _TR_corner, _BR_corner;

    void countConstraintsInPuzzleMatrix();
    bool checkSufficientEdgeTypes();
    bool checkSufficientCorners();
    bool checkSufficientOuterFrameConstraints();
};


#endif //ATPPUZZLESOLVER_SOLVABILITYVERIFIER_H
