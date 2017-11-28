//
// Created by Yoav on 28-Nov-17.
//

#ifndef ATPPUZZLESOLVER_CONSTRAINTSTREE_H
#define ATPPUZZLESOLVER_CONSTRAINTSTREE_H

#endif //ATPPUZZLESOLVER_CONSTRAINTSTREE_H

#include "const.h"
#include "PuzzlePiece.h"
#include <sstream>

class ConstraintsTable {
    unordered_map<std::string, set<int>> _table;

public:
    ConstraintsTable();
    void insertPiece(PuzzlePiece& p);
    set<int> getIDsFittingConstraints(int consts[4]);
};