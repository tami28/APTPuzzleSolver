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
    // Allocate buffer for solution matrix:
    PuzzleMatrix* solution;
    // Try and solve for every puzzle size:
    PuzzleMatrix pm(0,0);
    for (auto size : sizesVec){
        row = size.first;
        col = size.second;
        pm = PuzzleMatrix(row, col);
        _puzzle.get()->selAllPiecesValid(); //Before starting solve for size, set all pieces as "not used"
        //unordered_set<int> usedIDs; //TODO: note now calling 2nd version of _solveForSize! (overloaded)
        vector<int> usedIDs;
        next = std::make_unique<Step>(row, col); //TODO: fix frame step?
        solved = _solveForSize(pm, usedIDs); // Find a solution for size (row,col)
        if (solved) {
            break;
        }
    }
    if (solved){
        pm.toFile(outFilePath);

    }
    else {
        cout << "NO SOL" << endl; //todo rm
        (*ErrorList::getErrorList()).add(Error(COULD_NOT_FIND_SOLUTION));

    }

}


bool Solver::_solveForSize(PuzzleMatrix& pm, vector<int> usedIDs) {
    COUNT++;
//    std::cout << next.get()->i << ","<<next.get()->j << " " <<next.get()->nrow<<","<<next.get()->ncol<<std::endl;
    if (numPieces > MIN_NUM_PIECES_TO_CHECK_SUFFICIENT_CONSTRAINTS
        && usedIDs.size() > numPieces*(PIECES_RATIO_TO_CHECK_SUFFICIENT_CONSTRAINTS)
        &&  !(SolvabilityVerifier(_puzzle , pm, usedIDs)).verifySolvabilityConstraints()) {
            return false;
    }


    if (NULL!= pm.matrix[0][0].piece && pm.matrix[0][0].piece->getId() == 7) {
        int a = 3;
        if (NULL!= pm.matrix[0][1].piece && pm.matrix[0][1].piece->getId() == 4) {
            int a = 3;
            if (NULL!= pm.matrix[0][2].piece && pm.matrix[0][2].piece->getId() == 9) {
                int a = 3;
                if (NULL!= pm.matrix[1][0].piece && pm.matrix[1][0].piece->getId() == 6) {
                    int a = 3;
                    if (NULL!= pm.matrix[1][1].piece && pm.matrix[1][1].piece->getId() == 3) {
                        int a = 3;
                        if (NULL!= pm.matrix[1][2].piece && pm.matrix[1][2].piece->getId() == 1) {
                            int a = 3;
                            if (NULL!= pm.matrix[2][0].piece && pm.matrix[2][0].piece->getId() == 2) {
                                int a = 3;
                                if (NULL!= pm.matrix[2][1].piece && pm.matrix[2][1].piece->getId() == 5) {
                                    int a = 3;
                                    if (NULL!= pm.matrix[2][2].piece && pm.matrix[2][2].piece->getId() == 8) {
                                        int a = 3;
                                }
                            }
                        }
                    }
                }
            }
        }
    }}




    int constraints[4] = {NONE, NONE, NONE, NONE};
    pm.constraintsOfCell(next.get()->i,next.get()->j,constraints);
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


//TODO:  need to fix.
bool Solver::hasSingleRowColSolution(){
    return false; //todo: rm when fixed..
    PuzzleMatrix row_pm = PuzzleMatrix(1, _puzzle.get()->getSize());

    vector<int> usedIDs;


    _puzzle.get()->selAllPiecesValid(); //Before strating solve for size, set all pieces as "not used" todo: need this?
    next = std::make_unique<Step>(_puzzle.get()->getSize(), 1);
    if (_solveForSize(row_pm, usedIDs)){
        return true;
    }
    usedIDs.clear();
    PuzzleMatrix col_pm = PuzzleMatrix(_puzzle.get()->getSize(), 1);
    _puzzle.get()->selAllPiecesValid(); //Before strating solve for size, set all pieces as "not used"todo: need this?
    next = std::make_unique<Step>(1, _puzzle.get()->getSize() );
    if (_solveForSize(col_pm, usedIDs)){
        return true;
    }
    return false;
}

bool Solver::solverFinished(PuzzleMatrix& pm, vector<int> usedIDs){
    if ((!next.get()->nextStep()) && usedIDs.size() == numPieces){
        return true;
    }
    return false;
}

bool Step::nextStep(){
    if ( i == nrow-1){
        if (j == ncol-1){
            return false;
        }
        j++;
        return true;
    }
    if(j==ncol -1){
        i++;
        j = 0;
    } else{
        j++;
    }
    return true;
}

bool Step::prevStep(){
    if ( i == 0){
        if (j == 0){
            return false;
        }
        j--;
        return true;
    }
    if(j==0){
        i--;
        j = ncol-1;
    } else{
        j--;
    }
    return true;
}

bool StepFrame::nextStep(){
    //on first row till corner + 1:
    if ( i == 0){
        if (j < ncol-1){
            j++;
        } else{
            i++; //turn the corner
        }
        return true;
    }
    //last col till corner:
    if(j==ncol -1){
        if(i < nrow - 1){
            i++;
        } else{ //turn the corner:
            j--;
        }
        return true;
    }
    //last row:
    if (i == nrow-1){
        if (j>0){
            j--;
        }else{
            i--; //turn the corner
        }
        return true;
    }
    //first col:
    if (j==0){
        if(i>1){ //not to go over 0,0 again!
            i--;
        } else{ //finished frame, start inside!
            i=1;
            j=1;
        }
        return true;
    }
    //go over inside by rows:
    if ( i == nrow-2){
        if (j == ncol-2){
            return false;
        }
        j++;
        return true;
    }
    if(j==ncol -2){
        i++;
        j = 1;
    } else{
        j++;
    }
    return true;
}

bool StepFrame::prevStep(){
    //on first row till corner:
    if ( i == 0){
        if (j > 0){
            j--;
        } else{ //corner:
            return false;
        }
        return true;
    }
    //last col till corner:
    if(j==ncol -1){
        if(i >0){
            i--;
        } else{ //return on first row..
            j--;
        }
        return true;
    }
    //last row:
    if (i == nrow-1){
        if (j<ncol -1){
            j++;
        }else{ //turn the corner to last col
            i--;
        }
        return true;
    }
    //first col:
    if (j==0) {
        if (i < nrow - 1) { //not to go over 0,0 again!
            i++;
        } else {
            j++;
        }
        return true;
    }
    //    //go over inside by rows:
    if (i == 1 && j==1) {
        j--;
        return true;
    }
    if (j == 1){
        j = ncol -2;
        i--;
        return true;
    }else{
        j--;
    }
    return true;
}