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
 * todo: create this function..
 */
std::vector<pair<int, int>> Solver::getPossiblePuzzleSizes(){
    int puzzleSize = _puzzle.getSize();
    vector<pair<int,int>> result;
    int sqr = (int) sqrt(puzzleSize) + 1;
    for (int i = 1; i < sqr; i++) {
        if (puzzleSize % i == 0)
           result.push_back(pair<int,int>(i, puzzleSize/i));
           result.push_back(pair<int,int>(puzzleSize/i,i)); //TODO: currently we don't rotate pieces, so we need to check both n*m and m*n for m!=n (o/w will not find solution)
    }
    return result;
}


bool Solver::_solveForSize(PuzzleMatrix& pm, vector<int> indices, PuzzleMatrix* resultMatrix){
    //pm.print(); todo rm
    //cout << "*****************" << endl << endl; todo rm
    if (COUNTER++ % 50000 == 0){cout << COUNTER << endl;}; //TODO rm
    if (indices.size() == 0){
        *resultMatrix = pm; //TODO: make sure this COPIES pm to mem pointed to by result
        return true;
    }

    PuzzlePiece* piece;
    //First iteration (assign TL corner to matrix):
    if (indices.size() == _puzzle.getSize()) {
        for (int pieceIndex : indices){
            piece = _puzzle.getPieceAt(pieceIndex);
            if (piece->getEdge(TOP) == STRAIGHT && piece->getEdge(LEFT) == STRAIGHT){ // piece can be place at TL
                PuzzleMatrix new_pm = pm;
                new_pm.assignPieceToCell(piece, 0, 0);
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
    // Get all possible puzzle sizes:
    std::vector<pair<int, int>> sizesVec = getPossiblePuzzleSizes();
    vector<int> indices(_puzzle.getSize());
    // Fill indices vector with all relevant indices (1...numPieces)
    int i=1;
    std::iota(indices.begin(), indices.end(), 1);
    // Allocate buffer for solution matrix:
    PuzzleMatrix* solution = new PuzzleMatrix(row,col);
    // Try and solve for every puzzle size:
    for (auto size : sizesVec){
        row = size.first;
        col = size.second;
        //cout << "In Solver. now trying size: " << row << " "<<col << endl; //TODO rm
        PuzzleMatrix pm = PuzzleMatrix(row, col);
        COUNTER=0; //todo rm
        solved = _solveForSize(pm, indices, solution); // Found a solution for size (row,col)
        if (solved) {
            break;
        }
    }
    if (solved){
        solution->toFile("SOLUTION.txt"); //TODO: rename this file?
    }
}