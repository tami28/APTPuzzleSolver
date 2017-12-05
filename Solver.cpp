//
// Created by Yoav on 18-Nov-17.
//

#include "Solver.h"
#include <numeric>



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
    std::cout <<"finished checking constraints" << std::endl;
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


/*
 * TODO: if we don't use this, remove.
 * 1.check if pieces at the given indices have sufficient straight edges to cover all the non-covered
 * straight edges in PuzzleMatrix. if not, there's no point to continue exploring this solution.
 * 2.check sufficient cover of non-covered corners.
 * 3.check that there are enough make/female/straights in the remaining pieces.
 */

bool Solver::checkSufficientConstraints(vector<int> usedIDs, PuzzleMatrix *pm){
    if (withRotations){
        return true;
    }
    int straightLEFTs = 0, straightTOPs = 0, straightRIGHTs = 0, straightBOTTOMs = 0;
    PuzzlePiece* piece;
    bool TL_corner = false, BL_corner = false, TR_corner = false, BR_corner = false;
    std::unordered_map<int, int> sumConstraints;
    sumConstraints.insert({1, 0});
    sumConstraints.insert({-1, 0});
    sumConstraints.insert({0, 0});

    for (int i=1; (i <= numPieces) ; i++){
        if ((std::find(usedIDs.begin(), usedIDs.end(), i) != usedIDs.end())) {continue;}
        piece = _puzzle.get()->getPieceAt(i);
        if (piece->getConstraint(LEFT) == STRAIGHT) {
            straightLEFTs++;
            if ((!TL_corner) && piece->getConstraint(TOP) == STRAIGHT) { TL_corner = true;}
            if ((!BL_corner) && piece->getConstraint(BOTTOM) == STRAIGHT) { BL_corner = true;}
        }
        if (piece->getConstraint(TOP) == STRAIGHT) {straightTOPs++;}
        if (piece->getConstraint(RIGHT) == STRAIGHT) {
            straightRIGHTs++;
            if ((!TR_corner) && piece->getConstraint(TOP) == STRAIGHT) { TR_corner = true;}
            if ((!BR_corner) && piece->getConstraint(BOTTOM) == STRAIGHT) { BR_corner = true;}
        }
        if (piece->getConstraint(BOTTOM) == STRAIGHT) {straightBOTTOMs++;}

        sumConstraints[piece->getConstraint(LEFT)]++;
        sumConstraints[piece->getConstraint(TOP)]++;
        sumConstraints[piece->getConstraint(RIGHT)]++;
        sumConstraints[piece->getConstraint(BOTTOM)]++;
    }

    for (int c = FEMALE; c <= MALE; c++)
        {
            if (sumConstraints[(Constraints) c] < pm->requiredCounters[(Constraints) c])
            {
                return false;
            }
        }

    bool TL_required = (pm->matrix[0][0].piece == nullptr),
         BL_required = (pm->matrix[pm->getNrows()-1][0].piece == nullptr),
         TR_required = (pm->matrix[0][pm->getNcols()-1].piece == nullptr),
         BR_required = (pm->matrix[pm->getNrows()-1][pm->getNcols()-1].piece == nullptr);
    if ((BL_required && (!BL_corner)) ||
            (BR_required && (!BR_corner)) ||
            (TR_required && (!TR_corner)) ||
            (TL_required && (!TL_corner) )) {
            return false;
    }


    int requiredStraightUps=0,requiredStraightBottoms=0,requiredStraightLeft=0,requiredStraightRights=0;
    for (int i=0; i<pm->getNcols(); i++) {
        if (pm->matrix[0][i].piece == nullptr){requiredStraightUps++;}
        if (pm->matrix[pm->getNrows() - 1][i].piece == nullptr){requiredStraightBottoms++;}
    }
    for (int i=1; i<pm->getNrows()-1; i++) {
        if (pm->matrix[i][0].piece == nullptr){requiredStraightLeft++;}
        if (pm->matrix[i][pm->getNcols() - 1].piece == nullptr){requiredStraightRights++;}
    }
    if (straightRIGHTs < requiredStraightRights || straightTOPs < requiredStraightUps ||
        straightLEFTs < requiredStraightLeft || straightBOTTOMs < requiredStraightBottoms )
        return false;
    return true;
}


bool Solver::_solveForSize(PuzzleMatrix& pm, vector<int> usedIDs) {
    COUNT++;
    //TODO: decide about the constant here (0.5? 0.3?)
    if (numPieces > 30 && usedIDs.size() > numPieces*(0.5) && !checkSufficientConstraints(usedIDs, &pm)) { return false; }
    int constraints[4] = {NONE, NONE, NONE, NONE};
    pm.constraintsOfCell(next.get()->i,next.get()->j,constraints);
    unordered_set<string> badPieces;
    set<IDandRotation> relevantPieceIDs = _puzzle.get()->constraintsTable.getIDsFittingConstraints(constraints); //TODO: polymorphism
    int i;
    Rotate rotation;
    for (auto PieceIDandRotation : relevantPieceIDs){
        i = PieceIDandRotation.first;
        rotation = PieceIDandRotation.second;
        if (_isFitForCell(i, badPieces, usedIDs, rotation)){
            pm.assignPieceToCell(_puzzle.get()->getPieceAt(i),rotation, next.get()->i,next.get()->j);
            vector<int> newUsedIDs(usedIDs);
            newUsedIDs.push_back(i);
            if(solverFinished(pm, newUsedIDs)){
                return true;
            }
            if (_solveForSize(pm, newUsedIDs)){
                return true;
            }
            badPieces.insert((*_puzzle.get()->getPieceAt(i)).getConstraintStr(rotation)); //TODO: insert according to rotation.
            next.get()->prevStep();
        }
    }
    return false;
}


bool Solver::_isFitForCell(int i, std::unordered_set<string>& badPieces, vector<int> usedIDs, Rotate rotation){
    return (find(usedIDs.begin(),usedIDs.end(),i) == usedIDs.end() &&
            badPieces.find((*_puzzle.get()->getPieceAt(i)).getConstraintStr(rotation)) == badPieces.end());
}




bool Solver::piecefitsConstrains(PuzzlePiece& piece, char constraints[4]){
    for(int e = LEFT; e <= BOTTOM; ++e){
        if (!(0 == ((int)piece.getConstraint((Edge)e) +(int)constraints[e]) || constraints[e] == NONE)) {
            return false;
        }
    }
    return true;
}


//TODO
bool Solver::hasSingleRowColSolution(){
    PuzzleMatrix row_pm = PuzzleMatrix(1, _puzzle.get()->getSize());
    vector<int> indices(_puzzle.get()->getSize());
    // Fill indices vector with all relevant indices (1...numPieces)
    std::iota(indices.begin(), indices.end(), 1);
    _puzzle.get()->selAllPiecesValid(); //Before strating solve for size, set all pieces as "not used"
    next = std::make_unique<Step>(_puzzle.get()->getSize(), 1);
    if (_solveForSize(row_pm, indices)){
        return true;
    }

    PuzzleMatrix col_pm = PuzzleMatrix(_puzzle.get()->getSize(), 1);
    // Fill indices vector with all relevant indices (1...numPieces)
    std::iota(indices.begin(), indices.end(), 1);
    _puzzle.get()->selAllPiecesValid(); //Before strating solve for size, set all pieces as "not used"
    next = std::make_unique<Step>(1, _puzzle.get()->getSize());
    if (_solveForSize(col_pm, indices)){
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