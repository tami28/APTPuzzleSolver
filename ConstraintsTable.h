//
// Created by Yoav on 28-Nov-17.
//

#ifndef ATPPUZZLESOLVER_CONSTRAINTSTREE_H
#define ATPPUZZLESOLVER_CONSTRAINTSTREE_H

#endif //ATPPUZZLESOLVER_CONSTRAINTSTREE_H

#include "const.h"
#include "PuzzlePiece.h"
#include <sstream>
#include <unordered_map>

typedef pair<int, Rotate> IDandRotation;

class ConstraintsTable {
public:

    int getKey(int right, int top, int left, int bottom);
public:
    ConstraintsTable();
    void insertPiece(PuzzlePiece& p);
    set<IDandRotation> getIDsFittingConstraints(int consts[4]);
    std::unordered_map< int, set<IDandRotation> > _table;
};