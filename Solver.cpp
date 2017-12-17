//
// Created by Yoav on 18-Nov-17.
//

#include "Solver.h"




//Solver::Solver(){} //empty c'tor



/*
 * returns a vector of all pairs of ints r,c s.t r*c == puzzle.get_Size()
 */
std::vector<pair<int, int>> Solver::getPossiblePuzzleSizes(){
    vector<pair<int,int>> result;
    _puzzle.get()->getPossibleSizes(result);
    return result;
}



void Solver::solve(){
    int row, col;
    bool solved;

    //Check:
    if (0 == ErrorList::getNumErrors()){
        //check for wrong-num-of-straight-edges-error:
        _puzzle.get()->checkStraightEdges();
        //check for missing corner error:
        if (!(_puzzle.get()->isPrime(_puzzle.get()->getSize()) || _puzzle.get()->getSize()== 1 || hasSingleRowColSolution())) {
            _puzzle.get()->checkCorners();
        }
        //check for sum-not-zero error:
        if(0 != _puzzle.get()->getTotalSum()){
            (*ErrorList::getErrorList()).add(Error(SUM_EDGES_NOT_ZERO));
        }
    }
    if (ErrorList::getNumErrors() > 0) {
        return;
    }
    // Get all possible puzzle sizes:
    std::vector<pair<int, int>> sizesVec = getPossiblePuzzleSizes();
    vector<int> indices(_puzzle.get()->getSize());
    // Fill indices vector with all relevant indices (1...numPieces)
    std::iota(indices.begin(), indices.end(), 1);
    // Try and solve for every puzzle size:
    PuzzleMatrix pm(0,0);
    for (auto size : sizesVec){
        row = size.first;
        col = size.second;
        pm = PuzzleMatrix(row, col);
        vector<int> usedIDs;
        setStep(row, col);
        solved = _solveForSize(pm, usedIDs); // Find a solution for size (row,col)
        if (solved) {
            break;
        }
    }
    if (solved){
        pm.toFile(outFilePath);

    }
    else {
        (*ErrorList::getErrorList()).add(Error(COULD_NOT_FIND_SOLUTION));

    }

}


bool Solver::_solveForSize(PuzzleMatrix& pm, vector<int> usedIDs) {
    if (_puzzle.get()->getSize() > MIN_NUM_PIECES_TO_CHECK_SUFFICIENT_CONSTRAINTS
        && usedIDs.size() > _puzzle.get()->getSize()*(PIECES_RATIO_TO_CHECK_SUFFICIENT_CONSTRAINTS)
        &&  !(SolvabilityVerifier(_puzzle , pm, usedIDs)).verifySolvabilityConstraints()) {
            return false;
    }

    int constraints[4] = {NONE, NONE, NONE, NONE};
    pm.constraintsOfCell(next.get()->i,next.get()->j,constraints, next.get()->getType());
    unordered_set<int> badPieces;
    set<IDandRotation> relevantPieceIDs = _puzzle.get()->constraintsTable.getIDsFittingConstraints(constraints);
    int i;
    Rotate rotation;
    for (auto PieceIDandRotation : relevantPieceIDs){
        i = PieceIDandRotation.first;
        rotation = PieceIDandRotation.second;
        if (_isFitForCell(i, badPieces, usedIDs, rotation)){
            std::map<Constraints , int> requiredCountersSnapshot = pm.getRequiredCounters(); //TODO: bad hack.. change
            std::map<outerFrameConstraints, int> requieredFrameConstraintsSnapshot = pm.getRequiredFrameConstraints();//TODO: bad hack.. change
            pm.assignPieceToCell(_puzzle.get()->getPieceAt(i),rotation, next.get()->i,next.get()->j);
            vector<int> newUsedIDs(usedIDs);
            newUsedIDs.push_back(i);
            if(solverFinished(pm, newUsedIDs)){
                return true;
            }
            if (_solveForSize(pm, newUsedIDs)){
                return true;
            }
            pm.setRequiredCounters(requiredCountersSnapshot);
            pm.setRequiredFrameConstraints(requieredFrameConstraintsSnapshot);
            next.get()->prevStep();
            badPieces.insert((*_puzzle.get()->getPieceAt(i)).getConstraintsKey(rotation));
        }
    }
    return false;
}


bool Solver::_isFitForCell(int i, std::unordered_set<int>& badPieces, vector<int> usedIDs, Rotate rotation){
    return (find(usedIDs.begin(),usedIDs.end(),i) == usedIDs.end() &&
            badPieces.find((*_puzzle.get()->getPieceAt(i)).getConstraintsKey(rotation)) == badPieces.end());
}




bool Solver::piecefitsConstrains(PuzzlePiece& piece, char constraints[4]){
    for(int e = LEFT; e <= BOTTOM; ++e){
        if (!(0 == ((int)piece.getConstraint((Edge)e) +(int)constraints[e]) || constraints[e] == NONE)) {
            return false;
        }
    }
    return true;
}


bool Solver::hasSingleRowColSolution(){
    PuzzleMatrix row_pm = PuzzleMatrix(1, _puzzle.get()->getSize());
    vector<int> usedIDs;


    next = std::make_unique<Step>(1, _puzzle.get()->getSize());
    if (_solveForSize(row_pm, usedIDs)){
        return true;
    }
    usedIDs.clear();
    PuzzleMatrix col_pm = PuzzleMatrix(_puzzle.get()->getSize(), 1);
    next = std::make_unique<Step>(_puzzle.get()->getSize(),1);
    if (_solveForSize(col_pm, usedIDs)){
        return true;
    }
    return false;
}

bool Solver::solverFinished(PuzzleMatrix& pm, vector<int> usedIDs){
    if ((!next.get()->nextStep()) && usedIDs.size() == _puzzle.get()->getSize()){
        return true;
    }
    return false;
}


void Solver::setStep(int nrow, int ncol){
    //Decide which is the best stepper, meaning by what order to go:
    int colDiff = _puzzle.get()->numStraightEdges(LEFT) - nrow;
    int rowDiff = _puzzle.get()->numStraightEdges(TOP) - ncol;
//    if (colDiff >= rowDiff){
//        next = std::make_unique<Step>(nrow,ncol);
//    } else{
//        next = std::make_unique<StepCol>(nrow,ncol);
//    }
    if (nrow <= 2){
        next = std::make_unique<Step>(nrow,ncol);
        return;
    }
    if(ncol <= 2){
        next = std::make_unique<StepCol>(nrow,ncol);
        return;
    }
    next = std::make_unique<StepFrame>(nrow, ncol);


}

