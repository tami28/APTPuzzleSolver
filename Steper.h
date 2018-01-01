//
// Created by Tami on 17/12/2017.
//
#include "const.h"
#ifndef ATPPUZZLESOLVER_STEPER_H
#define ATPPUZZLESOLVER_STEPER_H


enum StepType{ROW, COL, FRAME};

class StepRow{
    friend class Solver;
protected:
    int i = 0;
    int j =0 ;
    int nrow = 0;
    int ncol = 0;
public:
    /*
     * update i&j to the steppersMap cell to go over. If we are at the end of the puzzle, meaning no steppersMap step - return false.
     */
    virtual bool nextStep();
    /*
     * update i&j to the previous cell we wentover. If we are at the start of the puzzle, meaning no prev step s- return false.
     */
    virtual bool prevStep();
    StepRow(){};
    StepRow(int k, int m) : nrow(k), ncol(m){};
    virtual StepType getType();
};

/*
 * Used to go over frame, then by row inside.
 */
class StepFrame :public StepRow{
    friend class Solver;
public:
    bool nextStep();
    bool prevStep();
    StepType getType();
    StepFrame(int n, int m) : StepRow(n,m){};

};

class StepCol : public StepRow{
    friend  class Solver;
public:
    bool nextStep();
    bool prevStep();
    StepType getType();
    StepCol(int n, int m) : StepRow(n,m){};
};

#endif //ATPPUZZLESOLVER_STEPER_H
