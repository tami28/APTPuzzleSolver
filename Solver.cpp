//
// Created by Yoav on 18-Nov-17.
//

#include "Solver.h"

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
    //cout << "now attemting :\n" << pm.toString() << endl; // todo rm
    //cout << pm.toString() << endl;
    //cout << "*****************" << endl << endl; todo rm
    //COUNTER++; //todo rm
    if (++COUNTER % 100000 == 0){cout << COUNTER << endl;}; //TODO rm
    //if (memoizationSet.size() % 1 == 0){cout << memoizationSet.size() << endl;}; //TODO rm

    PuzzlePiece* piece;
    string pmAsString;

    if (indices.size() == 0){
        *resultMatrix = pm;
        return true;
    }

    // Testing if pieces at given indices have sufficient straight edges to cover all straight edged that should be covered:
    if (!checkSufficientStraightEdges(indices, &pm)){
        return false;
    }

    //First iteration (assign TL corner to matrix):
    if (indices.size() == _puzzle.getSize()) {
        for (int pieceIndex : indices){
            piece = _puzzle.getPieceAt(pieceIndex);
            if (piece->getEdge(TOP) == STRAIGHT && piece->getEdge(LEFT) == STRAIGHT){ // piece can be place at TL
                PuzzleMatrix new_pm = pm;
                new_pm.assignPieceToCell(piece, 0, 0);
                pmAsString = new_pm.toString();
                if (memoizationSet.find(pmAsString) !=  memoizationSet.end()) { //We have already tried this subSolution.
                    continue;
                }
                else {
                    memoizationSet.insert(pmAsString);
                }
                vector<int> new_indices = indices;
                new_indices.erase(find(new_indices.begin(), new_indices.end(), pieceIndex));
                if (_solveForSize(new_pm, new_indices, resultMatrix)){
                    return true;
                }
            }
        }
    }



    for (int pieceIndex : indices){
        for (pair<int,int> frontierCell : pm.frontierCells){
            // Attempt to insert piece at index pieceIndex to frontierCell:
            piece = _puzzle.getPieceAt(pieceIndex);
            if (pm.isFit(piece, frontierCell.first, frontierCell.second)){
                PuzzleMatrix new_pm = pm;
                new_pm.assignPieceToCell(piece, frontierCell.first, frontierCell.second);
                pmAsString = new_pm.toString();
                if (memoizationSet.find(pmAsString) !=  memoizationSet.end()) { //We have already tried this subSolution.
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
    vector<int> indices(_puzzle.getSize());
    // Fill indices vector with all relevant indices (1...numPieces)
    int i=1;
    std::iota(indices.begin(), indices.end(), 1);
    // Allocate buffer for solution matrix:
    PuzzleMatrix* solution = new PuzzleMatrix(row,col);
    // Try and solve for every puzzle size:
    COUNTER=0; //todo rm
    for (auto size : sizesVec){
        row = size.first;
        col = size.second;
        //cout << "In Solver. now trying size: " << row << " "<<col << endl; //TODO rm
        PuzzleMatrix pm = PuzzleMatrix(row, col);

        solved = _solveForSize(pm, indices, solution); // Found a solution for size (row,col)
        if (solved) {
            break;
        }
    }
    if (solved){
        solution->toFile("SOLUTION.txt"); //TODO: rename this file?
    }

}


/*
 * check if pieces at the given indices have sufficient straight edges to cover all the non-covered
 * straight edges in PuzzleMatrix. if not, there's no point to continue exploring this solution.
 */
bool Solver::checkSufficientStraightEdges(vector<int> indices, PuzzleMatrix* pm){
    int straightLEFTs = 0, straightTOPs = 0, straightRIGHTs = 0, straightBOTTOMs = 0;
    PuzzlePiece* piece;
    for (auto i: indices){
        piece = _puzzle.getPieceAt(i);
        if (piece->getEdge(LEFT) == STRAIGHT) {straightLEFTs++;}
        if (piece->getEdge(TOP) == STRAIGHT) {straightTOPs++;}
        if (piece->getEdge(RIGHT) == STRAIGHT) {straightRIGHTs++;}
        if (piece->getEdge(BOTTOM) == STRAIGHT) {straightBOTTOMs++;}
    }
    int requiredStraightUps=0,requiredStraightBottoms=0,requiredStraightLeft=0,requiredStraightRights=0;
    for (int i=0; i<pm->getNcols(); i++) {
        if (pm->matrix[0][i].piece == NULL){requiredStraightUps++;}
        if (pm->matrix[pm->getNrows() - 1][i].piece == NULL){requiredStraightBottoms++;}
    }
    for (int i=1; i<pm->getNrows()-1; i++) {
        if (pm->matrix[i][0].piece == NULL){requiredStraightLeft++;}
        if (pm->matrix[i][pm->getNcols() - 1].piece == NULL){requiredStraightRights++;}
    }
    if (straightRIGHTs < requiredStraightRights || straightTOPs < requiredStraightUps ||
        straightLEFTs < requiredStraightLeft || straightBOTTOMs < requiredStraightBottoms )
        return false;
    return true;
}
