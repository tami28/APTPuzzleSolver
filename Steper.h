//
// Created by Tami on 17/12/2017.
//

#ifndef ATPPUZZLESOLVER_STEPER_H
#define ATPPUZZLESOLVER_STEPER_H


class Step{
    friend class Solver;
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
public:
    bool nextStep();
    bool prevStep();
    StepFrame(int n, int m) : Step(n,m){};

};

class StepCol : public Step{
    friend  class Solver;
public:
    bool nextStep();
    bool prevStep();
    StepCol(int n, int m) : Step(n,m){};
};

#endif //ATPPUZZLESOLVER_STEPER_H
