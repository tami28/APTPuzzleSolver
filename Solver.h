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


class Solver{
private:
    Puzzle _puzzle;

public:
    Solver();
    Solver(Puzzle& p);
    void setPuzzle(Puzzle& p);

    //TODO: destructors
};

//solution table
//Solution finder gets puzzle & solution table and return a table of pieces representing the solution
//Solver will have function that goes over possible legit sizes and call SolutionFinder

#endif /* SOLVER_H_ */
