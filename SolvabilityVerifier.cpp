//
// Created by Yoav on 11-Dec-17.
//

#include "SolvabilityVerifier.h"


/* Performs three checks to for solvability:
 * 1. remaining pieces have enough corners to cover all uncovered corners.
 * 2. remaining pieces have enough straight edges to cover all uncovered outer-frame cells.
 * 3. remaining pieces have enough of each edge type (-1,0,1) to account for requirements in puzzleMatrix.
 * returns true iff all above are true.
 */
bool SolvabilityVerifier::verifySolvabilityConstraints(){
    if (withRotations){ return true; }

    _TL_corner = false;
    _BL_corner = false;
    _TR_corner = false;
    _BR_corner = false;

    countConstraintsInPuzzleMatrix();
    return ( checkSufficientOuterFrameConstraints() &&
            checkSufficientCorners() &&
             checkSufficientEdgeTypes() );
}

void SolvabilityVerifier::countConstraintsInPuzzleMatrix(){
    PuzzlePiece* piece;
    _sumConstraints.insert({1, 0});
    _sumConstraints.insert({-1, 0});
    _sumConstraints.insert({0, 0});
    _sumOuterFrameEdges.insert({outerFrameConstraints::LEFT_STRAIGHT, 0});
    _sumOuterFrameEdges.insert({outerFrameConstraints::TOP_STRAIGHT, 0});
    _sumOuterFrameEdges.insert({outerFrameConstraints::RIGHT_STAIGHT, 0});
    _sumOuterFrameEdges.insert({outerFrameConstraints::BOTTOM_STRAIGHT, 0});
    for (int i=1; (i <= numPieces) ; i++){
        if ((std::find(_usedIDs.begin(), _usedIDs.end(), i) != _usedIDs.end())) {continue;}
        piece = _puzzle.get()->getPieceAt(i);
        if (piece->getConstraint(LEFT) == STRAIGHT) {
            _sumOuterFrameEdges[outerFrameConstraints::LEFT_STRAIGHT]++;
            if ((!_TL_corner) && piece->getConstraint(TOP) == STRAIGHT) { _TL_corner = true;}
            if ((!_BL_corner) && piece->getConstraint(BOTTOM) == STRAIGHT) { _BL_corner = true;}
        }
        if (piece->getConstraint(TOP) == STRAIGHT) {_sumOuterFrameEdges[outerFrameConstraints::TOP_STRAIGHT]++;}
        if (piece->getConstraint(RIGHT) == STRAIGHT) {
            _sumOuterFrameEdges[outerFrameConstraints::RIGHT_STAIGHT]++;
            if ((!_TR_corner) && piece->getConstraint(TOP) == STRAIGHT) { _TR_corner = true;}
            if ((!_BR_corner) && piece->getConstraint(BOTTOM) == STRAIGHT) { _BR_corner = true;}
        }
        if (piece->getConstraint(BOTTOM) == STRAIGHT) {_sumOuterFrameEdges[outerFrameConstraints::BOTTOM_STRAIGHT]++;}

        _sumConstraints[piece->getConstraint(LEFT)]++;
        _sumConstraints[piece->getConstraint(TOP)]++;
        _sumConstraints[piece->getConstraint(RIGHT)]++;
        _sumConstraints[piece->getConstraint(BOTTOM)]++;
    }
}

/*
 * check that remaining pieces have enough of every edge-type (-1,0,1) to account for requirements of puzzleMatrix.
 */
bool SolvabilityVerifier::checkSufficientEdgeTypes() {
    for (int c = FEMALE; c <= MALE; c++){
        if (_sumConstraints[(Constraints) c] < _pm.requiredCounters[(Constraints) c]) { return false; }
    }
    return true;
}
/*
 * check that remaining pieces have enough of every corner to account for corner requirements of puzzleMatrix.
*/
bool SolvabilityVerifier::checkSufficientCorners() {
    if (_pm.isCornerRequired(Corners::TL) && (!_TL_corner)) {return false;}
    if (_pm.isCornerRequired(Corners::TR) && (!_TR_corner)) {return false;}
    if (_pm.isCornerRequired(Corners::BR) && (!_BR_corner)) {return false;}
    if (_pm.isCornerRequired(Corners::BL) && (!_BL_corner)) {return false;}
    return true;
}

bool SolvabilityVerifier::checkSufficientOuterFrameConstraints(){
    // Verify that there are enough topLeft, bottomRight, etc to account for all required outer-frame constraints:
    for (int c = outerFrameConstraints::LEFT_STRAIGHT ; c < outerFrameConstraints::FINAL ; c++){
        if ( _sumOuterFrameEdges[outerFrameConstraints (c)] < _pm.getOuterFrameRequirementAt((outerFrameConstraints) c) ) {
            return false;
        }
    }
    return true;
}

