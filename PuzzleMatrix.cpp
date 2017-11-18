//
// Created by Yoav on 13-Nov-17.
//
#include "PuzzleMatrix.h"



PuzzleMatrix::PuzzleMatrix(int row, int col){
    nrows = row;
    ncols = col;
    matrix = CellArray2D(row, CellArray(col, Cell() ));
    //Initialize all constraints (giving STRAIGHT values to cells near edge of puzzle, and NONE to others):
    for (int i=0; i<row; i++){
        for (int j=0; j<col; j++){
            for (int k=0; k<4; k++){
                if (i == row-1 && k == Edge::BOTTOM){matrix[i][j].constraints[k] = Constraints::STRAIGHT;}
                else if (i == 0 && k == Edge::TOP){matrix[i][j].constraints[k] = Constraints::STRAIGHT;}
                else if (j == col-1 && k == Edge::RIGHT){matrix[i][j].constraints[k] = Constraints::STRAIGHT;}
                else if (j == 0 && k == Edge::LEFT){matrix[i][j].constraints[k] = Constraints::STRAIGHT;}
                else {matrix[i][j].constraints[k] = Constraints::NONE;}
            }
        }
    }
}

Constraints PuzzleMatrix::operator()(int row, int col, Edge edge){
        return matrix[row][col].constraints[edge];
}



/*
 * Update the constraints of a given neighbour, according to a specific edge of the piece that was
 * placed near that neighbour.
 */
void PuzzleMatrix::UpdateConstraintsOfNeighbour(PuzzlePiece* piece,Edge pieceEdgeToUpdateBy, Edge neighbourEdgeToUpdate,
                                                int neighbourRow, int neighbourCol) {
    switch (piece->getEdge(pieceEdgeToUpdateBy)) {
        case STRAIGHT:
            matrix[neighbourRow][neighbourCol].constraints[neighbourEdgeToUpdate] = STRAIGHT;
            break;
        case MALE:
            matrix[neighbourRow][neighbourCol].constraints[neighbourEdgeToUpdate] = FEMALE;
            break;
        case FEMALE:
            matrix[neighbourRow][neighbourCol].constraints[neighbourEdgeToUpdate] = MALE;
            break;
    }
}



/*
 * assign a given piece to a cell in the PuzzleMatrix, and updates the constraints of the neighbouring cells accordingly
 * assumes piece can be legally assigned there.
 */
void PuzzleMatrix::assignPieceToCell(PuzzlePiece* piece, int row, int col){
    matrix[row][col].piece = piece;
    //Update constraints of neighbours:
    if (row > 0){//cell has a neighbour above
        UpdateConstraintsOfNeighbour(piece, TOP, BOTTOM, row-1, col);
        }
    if (row < nrows - 1){//cell has a neighbour below
        UpdateConstraintsOfNeighbour(piece, BOTTOM, TOP, row+1, col);
    }
    if (col > 0){//cell has a neighbour on the left
        UpdateConstraintsOfNeighbour(piece, LEFT, RIGHT, row, col-1);
    }
    if (col < ncols - 1){//cell has a neighbour on the right
        UpdateConstraintsOfNeighbour(piece, RIGHT, LEFT, row, col+1);
    }

    updateFrontiers(row, col);
}

void PuzzleMatrix::updateFrontiers(int row, int col){
    if (row > 0 && matrix[row - 1][col].piece == nullptr){//Given cell has a vacant neighbour above
        frontierCells.insert(pair<int,int>(row-1,col));
    }
    if (row < nrows - 1 && matrix[row+1][col].piece == nullptr){//Given cell has a vacant neighbour below
        frontierCells.insert(pair<int,int>(row+1,col));
    }
    if (col > 0 && matrix[row][col-1].piece == nullptr){//Given cell has a vacant neighbour on the left
        frontierCells.insert(pair<int,int>(row,col-1));
    }
    if (col < ncols - 1 && matrix[row][col+1].piece == nullptr){//Given cell has a vacant neighbour on the right
        frontierCells.insert(pair<int,int>(row,col+1));
    }

    //Remove position of inserted cell from frontierCells, if it is there:
    auto iter = std::find( frontierCells.begin(), frontierCells.end(),
                           pair<int,int>(row,col) );
    if (iter != frontierCells.end()){
        frontierCells.erase(iter);

    }
}




/*
 * check if given PuzzlePiece can be fitted to PuzzleMatrix[row][col], according to constraints of that cell.
 * if piece does not fit or cell is full (i.e it's piece pointer != NULL) - returns False, else True.
 * if every non-NULL constraint matches the edge of the piece --> fits.
 */
bool PuzzleMatrix::isFit(PuzzlePiece* piece, int row, int col){
    if (matrix[row][col].piece != NULL){
        return false;
    }
    for (int i = LEFT; i<=BOTTOM; i++){ //iterate over constraints
        if (matrix[row][col].constraints[i] == NONE){
            continue;
        }
        switch (matrix[row][col].constraints[i]){
            case (NONE)://the i'th edge of the cell has no constraint.
                continue;
            case (FEMALE):
                if (piece->getEdge((Edge) i) != FEMALE){
                    return false;
                }
                break;
            case (MALE):
                if (piece->getEdge((Edge) i) != MALE){
                    return false;
                }
                break;
            case (STRAIGHT):
                if (piece->getEdge((Edge) i) != STRAIGHT){
                    return false;
                }
                break;
        }
    }
    return true;
}


