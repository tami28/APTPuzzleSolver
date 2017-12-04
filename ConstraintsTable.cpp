//
// Created by Yoav on 28-Nov-17.
//

#include "ConstraintsTable.h"


ConstraintsTable::ConstraintsTable(){
    for (int i = FEMALE; i <= NONE; i++)
        for (int j = FEMALE; j <= NONE; j++)
            for (int k = FEMALE; k <= NONE; k++)
                for (int m = FEMALE; m <= NONE; m++){
                    int key = getKey(i,j,k,m);
                    _table.insert( {key, set<IDandRotation>()} );
                }
}


void ConstraintsTable::insertPiece(PuzzlePiece& p){
    Constraints pieceFittingConstraints[4];
    int timesRotated = 0;
    do {
        for (int i = LEFT; i < LAST; i++) { pieceFittingConstraints[(i + timesRotated) % 4] = p.getConstraint((Edge) i); }
        int right, top, left, bottom;
        for (int i = 0; i <= 1; i++)
            for (int j = 0; j <= 1; j++)
                for (int k = 0; k <= 1; k++)
                    for (int m = 0; m <= 1; m++) {
                        right = pieceFittingConstraints[0];
                        top = pieceFittingConstraints[1];
                        left = pieceFittingConstraints[2];
                        bottom = pieceFittingConstraints[3];
                        if (i) { bottom = Constraints::NONE; }
                        if (j) { left = Constraints::NONE; }
                        if (k) { top = Constraints::NONE; }
                        if (m) { right = Constraints::NONE; }


                        int key = getKey(right, top, left, bottom);
                        _table.at(key).insert({p.getId(), (Rotate) (timesRotated*90)});
                    }
        timesRotated++;
    } while (withRotations && timesRotated <= 3);
}


set<IDandRotation> ConstraintsTable::getIDsFittingConstraints(int consts[4]){
    int key = getKey(consts[0], consts[1], consts[2],consts[3]);
    set<IDandRotation> res = _table.at(key);
    return res;
    }


int ConstraintsTable::getKey(int right, int top, int left, int bottom){
    return (right + 1)*1000 + (top + 1)*100 + (left + 1)*10 + (bottom + 1);
}