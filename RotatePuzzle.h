//
// Created by Tami on 05/12/2017.
//

#ifndef ATPPUZZLESOLVER_ROTATEPUZZLE_H
#define ATPPUZZLESOLVER_ROTATEPUZZLE_H

#include "Puzzle.h"

class RotatePuzzle :public Puzzle{
public:
    RotatePuzzle(string fileName): Puzzle(fileName){withRotations = true;};
    void getPossibleSizes(vector<pair<int,int>> & result);
    void checkCorners();

};


#endif //ATPPUZZLESOLVER_ROTATEPUZZLE_H
