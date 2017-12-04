//
// Created by Yoav on 18-Nov-17.
//

#include "Solver.h"



Solver::Solver(Puzzle& p){ //c'tor from Puzzle
    this->_puzzle = p;
    // Initialize vector of indices:
    for( int i = 1; i <= p.getSize(); i++ )
        this->indices.push_back( i );
}


/*
 * returns a vector of all pairs of ints r,c s.t r*c == puzzle.get_Size()
 */
std::vector<pair<int, int>> Solver::getPossiblePuzzleSizes(){
    vector<pair<int,int>> result;
    _puzzle.getPossibleSizes(result);
    return result;
}



void Solver::solve(){
    int row, col;
    bool solved;

    //Check:
    if (0 == ErrorList::getNumErrors()){
        //check for wrong-num-of-straight-edges-error:
        _puzzle.checkStraightEdges();
        //check for missing corner error:
        if (!(_puzzle.isPrime(_puzzle.getSize()) || _puzzle.getSize()== 1 || hasSingleRowColSolution())) {
            _puzzle.checkCorners();
        }
        //check for sum-not-zero error:
        if(0 != _puzzle.getTotalSum()){
            (*ErrorList::getErrorList()).add(Error(SUM_EDGES_NOT_ZERO));
        }
    }
    if (ErrorList::getNumErrors() > 0) {
        return;
    }
    // Get all possible puzzle sizes:
    std::vector<pair<int, int>> sizesVec = getPossiblePuzzleSizes();
    vector<int> indices(_puzzle.getSize());
    // Fill indices vector with all relevant indices (1...numPieces)
    std::iota(indices.begin(), indices.end(), 1);
    // Allocate buffer for solution matrix:
    PuzzleMatrix* solution;
    // Try and solve for every puzzle size:
    PuzzleMatrix pm(0,0);
    for (auto size : sizesVec) {
        row = size.first;
        col = size.second;
        pm = PuzzleMatrix(row, col);
        _puzzle.selAllPiecesValid(); //Before starting solve for size, set all pieces as "not used"
        //unordered_set<int> usedIDs; //TODO: note now calling 2nd version of _solveForSize! (overloaded)
        vector<int> usedIDs;
//        if (min(row,col)>2 &&_puzzle.totalStraightEdges() < numPieces) {//TODO: what it the right const here?
//            next = std::make_unique<StepFrame>(row, col);
//        //    std::cout<<"using frame"<<std::endl;
//            isFrame = true;
//        } else {
//            next = std::make_unique<Step>(row, col);
//        }
        next = std::make_unique<Step>(row, col);
        solved = _solveForSize(pm, usedIDs, 0, 0); // Find a solution for size (row,col)
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


/*
 * 1.check if pieces at the given indices have sufficient straight edges to cover all the non-covered
 * straight edges in PuzzleMatrix. if not, there's no point to continue exploring this solution.
 * 2.check sufficient cover of non-covered corners.
 * 3.check that there are enough make/female/straights in the remaining pieces.
 */

bool Solver::checkSufficientConstraints(vector<int> usedIDs, PuzzleMatrix *pm){

    //Don't bother checking for small puzzles or if we've just started the puzzle.
    //TODO: what is the best const here?
    if (numPieces < 30 || usedIDs.size() < numPieces*(0.5) ){
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
        piece = _puzzle.getPieceAt(i);
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


bool Solver::_solveForSize(PuzzleMatrix& pm, vector<int> usedIDs, int row, int col) {
    COUNT++;
    //TODO: decide about the constant here (0.5? 0.3?)
    if (!checkSufficientConstraints(usedIDs, &pm)) {
        //next.get()->prevStep();
//        std::cout <<" no consstrains, returned to:" << next.get()->i << " , "<<next.get()->j <<std::endl;
        return false;
    }
    int constraints[4] = {NONE, NONE, NONE, NONE};
    if(!isFrame){
        pm.constraintsOfCell(next.get()->i,next.get()->j,constraints);
    } else{
        pm.constraintsOfCellFrameStep(next.get()->i,next.get()->j,constraints);
    }

    unordered_set<string> badPieces;
    set<int> relevantPieceIDs = _puzzle.constraintsTable.getIDsFittingConstraints(constraints); //TODO: polymorphism
    for (int i : relevantPieceIDs){
        if (_isFitForCell(i, badPieces, usedIDs)){
            pm.assignPieceToCell(_puzzle.getPieceAt(i), next.get()->i,next.get()->j); //TODO should rotate before or hold rotationo
            vector<int> newUsedIDs(usedIDs);
            newUsedIDs.push_back(i);
            if(solverFinished(pm, newUsedIDs)){
                return true;
            }
            if ( col == next.get()->ncol-1){
                if(_solveForSize(pm, newUsedIDs, row+1, 0)){
                    return true;
                }
            } else{
                if(_solveForSize(pm, newUsedIDs, row, col+1)){
                    return true;
                }
            }


            badPieces.insert((*_puzzle.getPieceAt(i)).getConstraintStr());
            next.get()->prevStep();
        }
    }
//    next.get()->prevStep();

    return false;
}


bool Solver::_isFitForCell(int i, std::unordered_set<string>& badPieces, vector<int> usedIDs){
    return (find(usedIDs.begin(),usedIDs.end(),i) == usedIDs.end() &&
            badPieces.find((*_puzzle.getPieceAt(i)).getConstraintStr()) == badPieces.end());
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
    next = std::make_unique<Step>(1, _puzzle.getSize());
    PuzzleMatrix row_pm = PuzzleMatrix(1, _puzzle.getSize());
    vector<int> indices(_puzzle.getSize());
    // Fill indices vector with all relevant indices (1...numPieces)
    std::iota(indices.begin(), indices.end(), 1);
    _puzzle.selAllPiecesValid(); //Before strating solve for size, set all pieces as "not used"
    if (_solveForSize(row_pm, indices,0,0)){
        return true;
    }

    next = std::make_unique<Step>(_puzzle.getSize(), 1);
    PuzzleMatrix col_pm = PuzzleMatrix(_puzzle.getSize(), 1);
    // Fill indices vector with all relevant indices (1...numPieces)
    std::iota(indices.begin(), indices.end(), 1);
    _puzzle.selAllPiecesValid(); //Before strating solve for size, set all pieces as "not used"
    if (_solveForSize(col_pm, indices,0,0)){
        return true;
    }
    return false;
}

bool Solver::solverFinished(PuzzleMatrix& pm, vector<int>& usedIDs){
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