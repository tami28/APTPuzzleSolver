//
// Created by Yoav on 18-Nov-17.
//

#include "Solver.h"


// #TODO: condider adding an innitial solvability test, to check if there are enough corners/straight edges/sums of male-female etc.
Solver::Solver(){}; //empty c'tor

Solver::Solver(Puzzle& p){ //c'tor from Puzzle
    this->_puzzle = p;
    // Initialize vector of indices:
    for( int i = 1; i <= p.getSize(); i++ )
        this->indices.push_back( i );
};


/*
 * returns a vector of all pairs of ints r,c s.t r*c == puzzle.get_Size()
 */
std::vector<pair<int, int>> Solver::getPossiblePuzzleSizes(){
    int puzzleSize = _puzzle.getSize();
    vector<pair<int,int>> result;
    int sqr = (int) sqrt(puzzleSize) + 1;
    for (int i = 1; i < sqr; i++) {
        if (puzzleSize % i == 0) {
            result.push_back(pair<int, int>(i, puzzleSize / i));
            if (i != puzzleSize / i) {
                result.push_back(pair<int, int>(puzzleSize / i,
                                                i)); //TODO: currently we don't rotate pieces, so we need to check both n*m and m*n for m!=n (o/w will not find solution)
            }
        }
    }
    return result;
}


bool Solver::_solveForSize(PuzzleMatrix& pm, vector<int> indices, PuzzleMatrix* resultMatrix){
    //cout << "now solving :\n" << pm.toString() << endl; // todo rm
    //cout << "MEMIZset = \n";
    //for (auto s : memoizationSet) {cout << s << endl;}
    //cout << "\n**********************" << endl << endl; //todo rm
    COUNTER++; //todo rm
    //if (COUNTER % 10000 == 0)        {cout << COUNTER << endl;}; //TODO rm
    //if (memoizationSet.size() % 1 == 0){cout << memoizationSet.size() << endl;}; //TODO rm

    PuzzlePiece* piece;
    string pmAsString;

    if (indices.size() == 0){
        *resultMatrix = pm;
        return true;
    }

    // Testing if pieces at given indices have sufficient straight edges to cover all straight edged that should be covered:
    if (!checkSufficientConstraints(indices, &pm)){
        return false;
    }


    PuzzlePiece *piece1, *piece2, *piece3, *piece4;
    //TRY : 4 sides
    if (indices.size() == _puzzle.getSize()) {
        for (int piece1Index : indices){
            piece1 = _puzzle.getPieceAt(piece1Index);


            if (piece1->getConstraint(TOP) == STRAIGHT && piece1->getConstraint(LEFT) == STRAIGHT){ // piece can be place at TL
                vector<int> inds_wo_p1 = indices;
                inds_wo_p1.erase(find(inds_wo_p1.begin(), inds_wo_p1.end(), piece1Index));
                for (int piece2Index : inds_wo_p1){
                    piece2 = _puzzle.getPieceAt(piece2Index);
                    if (piece2->getConstraint(TOP) == STRAIGHT && piece2->getConstraint(RIGHT) == STRAIGHT){ // piece can be place at TR
                        vector<int> inds_wo_p2 = inds_wo_p1;
                        inds_wo_p2.erase(find(inds_wo_p2.begin(), inds_wo_p2.end(), piece2Index));
                        for (int piece3Index : inds_wo_p2) {
                            piece3 = _puzzle.getPieceAt(piece3Index);
                            if (piece3->getConstraint(BOTTOM) == STRAIGHT &&
                                piece3->getConstraint(LEFT) == STRAIGHT) { // piece can be place at BL
                                vector<int> inds_wo_p3 = inds_wo_p2;
                                inds_wo_p3.erase(find(inds_wo_p3.begin(), inds_wo_p3.end(), piece3Index));
                                for (int piece4Index : inds_wo_p3) {
                                    piece4 = _puzzle.getPieceAt(piece4Index);
                                    if (piece4->getConstraint(BOTTOM) == STRAIGHT &&
                                        piece4->getConstraint(RIGHT) == STRAIGHT) { // piece can be place at BR
                                        PuzzleMatrix new_pm = pm;
                                        new_pm.assignPieceToCell(piece1, 0, 0);
                                        new_pm.assignPieceToCell(piece2, 0, pm.getNcols()-1);
                                        new_pm.assignPieceToCell(piece3, pm.getNrows()-1, 0);
                                        new_pm.assignPieceToCell(piece4, pm.getNrows()-1, pm.getNcols()-1);
                                        //pmAsString = new_pm.toString();
                                        pmAsString = new_pm.encode();
                                        if (memoizationSet.find(pmAsString) !=  memoizationSet.end()) { //We have already tried this subSolution.
                                            continue;
                                            COUNT_MEMOIZATION_SUCCESSES++; // TODO rm
                                        }
                                        else {
                                            memoizationSet.insert(pmAsString);
                                        }
                                        vector<int> new_indices = inds_wo_p3;
                                        new_indices.erase(find(new_indices.begin(), new_indices.end(), piece4Index));
                                        if (_solveForSize(new_pm, new_indices, resultMatrix)){
                                            return true;
                                        }

                                    }

                            }
                        }
                        }
                    }}}}}





//    First iteration (assign TL corner to matrix):
//    if (indices.size() == _puzzle.getSize()) {
//        for (int pieceIndex : indices){
//            piece = _puzzle.getPieceAt(pieceIndex);
//            if (piece->getConstraint(TOP) == STRAIGHT && piece->getConstraint(LEFT) == STRAIGHT){ // piece can be place at TL
//                PuzzleMatrix new_pm = pm;
//                new_pm.assignPieceToCell(piece, 0, 0);
//                pmAsString = new_pm.toString();
//
//                if (memoizationSet.find(pmAsString) !=  memoizationSet.end()) { //We have already tried this subSolution.
//                    continue;
//                }
//                else {
//                    memoizationSet.insert(pmAsString);
//                }
//                vector<int> new_indices = indices;
//                new_indices.erase(find(new_indices.begin(), new_indices.end(), pieceIndex));
//                if (_solveForSize(new_pm, new_indices, resultMatrix)){
//                    return true;
//                }
//            }
//        }
//    }



    for (int pieceIndex : indices){
        piece = _puzzle.getPieceAt(pieceIndex);
        for (pair<int,int> frontierCell : pm.frontierCells){
            // Attempt to insert piece at index pieceIndex to frontierCell:
            if (pm.isFit(piece, frontierCell.first, frontierCell.second)){
                PuzzleMatrix new_pm = pm;
                new_pm.assignPieceToCell(piece, frontierCell.first, frontierCell.second);
                //pmAsString = new_pm.toString();
                pmAsString = new_pm.encode();
                //cout<< "trying:\n";//TODO rm
                //cout << pmAsString << "\n";//TODO rm
                if (memoizationSet.find(pmAsString) !=  memoizationSet.end()) { //We have already tried this subSolution.
                    //cout << "found in meme table!\n";//TODO rm
                    COUNT_MEMOIZATION_SUCCESSES++; // TODO rm
                    continue;
                }
                else {
                    memoizationSet.insert(pmAsString);
                }
                vector<int> new_indices = indices;
                new_indices.erase(find(new_indices.begin(), new_indices.end(), pieceIndex));
                if (_solveForSize(new_pm, new_indices, resultMatrix)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Solver::solve(){
    int row, col;
    bool solved;
    memoizationSet.clear();
    // Get all possible puzzle sizes:
    std::vector<pair<int, int>> sizesVec = getPossiblePuzzleSizes();
    //sizesVec = {pair<int, int>(4,3)}; //todo rm
    vector<int> indices(_puzzle.getSize());
    // Fill indices vector with all relevant indices (1...numPieces)
    int i=1;
    std::iota(indices.begin(), indices.end(), 1);
    // Allocate buffer for solution matrix:
    PuzzleMatrix* solution;
    // Try and solve for every puzzle size:
    COUNTER=0; //todo rm
    for (auto size : sizesVec){
        row = size.first;
        col = size.second;
        solution = new PuzzleMatrix(row,col);
        //cout << "In Solver. now trying size: " << row << " "<<col << endl; //TODO rm
        PuzzleMatrix pm = PuzzleMatrix(row, col);

        solved = _solveForSize(pm, indices, solution); // Found a solution for size (row,col)
        if (solved) {
            break;
        } else {delete solution;}
    }
    if (solved){
        solution->toFile("SOLUTION.txt"); //TODO: rename this file?
        delete solution;
    }

}


/*
 * 1.check if pieces at the given indices have sufficient straight edges to cover all the non-covered
 * straight edges in PuzzleMatrix. if not, there's no point to continue exploring this solution.
 * 2.check sufficient cover of non-covered corners.
 * 3.check that there are enough make/female/straights in the remaining pieces.
 */
bool Solver::checkSufficientConstraints(vector<int> indices, PuzzleMatrix *pm){
    int straightLEFTs = 0, straightTOPs = 0, straightRIGHTs = 0, straightBOTTOMs = 0;
    PuzzlePiece* piece;
    bool TL_corner = false, BL_corner = false, TR_corner = false, BR_corner = false;
    std::map<Constraints , int> sumConstraints;
    for (auto i: indices){
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
//                cout << "\nconstraints mismatch in matrix: \n" << endl; //todo: rm
//                pm->print(); //todo: rm
//                cout << "\nindices:";
//                for (auto i: indices) {cout <<i<< " ";}
//                cout << "\n";
                return false;
            }
        }


    bool TL_required = (pm->matrix[0][0].piece == NULL),
         BL_required = (pm->matrix[pm->getNrows()-1][0].piece == NULL),
         TR_required = (pm->matrix[0][pm->getNcols()-1].piece == NULL),
         BR_required = (pm->matrix[pm->getNrows()-1][pm->getNcols()-1].piece == NULL);
    if (BL_required && (!BL_corner) ||
        BR_required && (!BR_corner) ||
        TR_required && (!TR_corner) ||
        TL_required && (!TL_corner) ) {
            //cout << "corner redundancy in matrix: \n" << endl; todo: rm
            //pm->print(); todo: rm
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

