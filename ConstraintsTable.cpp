//
// Created by Yoav on 28-Nov-17.
//

#include "ConstraintsTable.h"


ConstraintsTable::ConstraintsTable(){
    for (int i = FEMALE; i <= NONE; i++)
        for (int j = FEMALE; j <= NONE; j++)
            for (int k = FEMALE; k <= NONE; k++)
                for (int m = FEMALE; m <= NONE; m++){
                    std::stringstream key;
                    key << i << j << k << m;
                    _table.insert( {key.str(), set<int>()} );
                }
}


void ConstraintsTable::insertPiece(PuzzlePiece& p){
    Constraints pieceFittingConstraints[4];
    for (int i = LEFT; i < LAST; i++) { pieceFittingConstraints[i] = p.getOppositeConstraint( (Edge) i ); }

    for (int i = 0; i <= 1; i++)
        for (int j = 0; j <= 1; j++)
            for (int k = 0; k <= 1; k++)
                for (int m = 0; m <= 1; m++){
                    if (i) { pieceFittingConstraints[3] = Constraints::NONE; }
                    if (j) { pieceFittingConstraints[2] = Constraints::NONE; }
                    if (k) { pieceFittingConstraints[1] = Constraints::NONE; }
                    if (m) { pieceFittingConstraints[0] = Constraints::NONE; }

                    std::stringstream key;
                    key << pieceFittingConstraints[0] << pieceFittingConstraints[1] << pieceFittingConstraints[2] << pieceFittingConstraints[3];
                    _table.at(key.str()).insert(p.getId());
                }
}


set<int> ConstraintsTable::getIDsFittingConstraints(int consts[4]){
    std::stringstream key;
    key << consts[0] << consts[1] << consts[2] << consts[3];
    return _table.at(key.str());
    }