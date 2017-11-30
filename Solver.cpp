//
// Created by Yoav on 18-Nov-17.
//

#include "Solver.h"
#include <numeric>



Solver::Solver(){} //empty c'tor

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
    memoizationSet.clear(); //left for a different type of solution we might add

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
    for (auto size : sizesVec){
        row = size.first;
        col = size.second;
        solution = new PuzzleMatrix(row,col);
        pm = PuzzleMatrix(row, col);
        _puzzle.selAllPiecesValid(); //Before starting solve for size, set all pieces as "not used"
        //unordered_set<int> usedIDs; //TODO: note now calling 2nd version of _solveForSize! (overloaded)
        vector<int> usedIDs;
        solved = _solveForSize(pm, usedIDs, solution, 0, 0); // Find a solution for size (row,col)
        if (solved) {
            break;
        } else {delete solution;}
    }
    if (solved){
        pm.toFile(outFilePath);
        delete solution;
    }
    else {
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






bool Solver::_solveForSize(PuzzleMatrix& pm, vector<int> usedIDs, PuzzleMatrix *result, int row, int col) {
    COUNT++;
    if(row == (pm.getNrows()) && col== 0 && usedIDs.size() == numPieces){
        return true;
    }
    //TODO: decide about the constant here (0.5? 0.3?)
    if (numPieces > 30 && usedIDs.size() > numPieces*(0.5) && !checkSufficientConstraints(usedIDs, &pm)) { return false; }
    int constraints[4] = {NONE, NONE, NONE, NONE};
    pm.constraintsOfCell(row,col,constraints);
    unordered_set<string> badPieces;
    set<int> relevantPieceIDs = _puzzle.constraintsTable.getIDsFittingConstraints(constraints);
    for (int i : relevantPieceIDs){
        if ( std::find(usedIDs.begin(), usedIDs.end(), i) != usedIDs.end() ) { continue; }
        if (badPieces.find((*_puzzle.getPieceAt(i)).getConstraintStr()) == badPieces.end()){
            pm.assignPieceToCell(_puzzle.getPieceAt(i), row,col);
            vector<int> newUsedIDs(usedIDs);
            newUsedIDs.push_back(i);
            if( col < (pm.getNcols()-1)){
                if (_solveForSize(pm, newUsedIDs, result, row, col+1)){
                    return true;
                }
            } else{
                if(_solveForSize(pm, newUsedIDs, result, row+1, 0)){
                    return true;
                }
            }
            badPieces.insert((*_puzzle.getPieceAt(i)).getConstraintStr());
        }
    }
    return false;
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
    PuzzleMatrix row_pm = PuzzleMatrix(1, _puzzle.getSize());
    vector<int> indices(_puzzle.getSize());
    // Fill indices vector with all relevant indices (1...numPieces)
    std::iota(indices.begin(), indices.end(), 1);
    _puzzle.selAllPiecesValid(); //Before strating solve for size, set all pieces as "not used"
    if (_solveForSize(row_pm, indices, &row_pm, 0, 0)){
        return true;
    }

    PuzzleMatrix col_pm = PuzzleMatrix(_puzzle.getSize(), 1);
    // Fill indices vector with all relevant indices (1...numPieces)
    std::iota(indices.begin(), indices.end(), 1);
    _puzzle.selAllPiecesValid(); //Before strating solve for size, set all pieces as "not used"
    if (_solveForSize(col_pm, indices, &col_pm, 0, 0)){
        return true;
    }
    return false;
}