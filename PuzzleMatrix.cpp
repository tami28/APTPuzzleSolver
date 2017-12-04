//
// Created by Yoav on 13-Nov-17.
//
#include <fstream>
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
    requiredCounters[STRAIGHT] = 2*nrows + 2*ncols;
    requiredCounters[MALE] = 0;
    requiredCounters[FEMALE] = 0;

}

Constraints PuzzleMatrix::operator()(int row, int col, Edge edge){
        return matrix[row][col].constraints[edge];
}


/*
 * copy c'tor
 */
PuzzleMatrix::PuzzleMatrix(const PuzzleMatrix &other){
    nrows = other.getNrows();
    ncols = other.getNcols();
    this->requiredCounters = other.requiredCounters;
    this->matrix = other.matrix;
}

int PuzzleMatrix::getNrows() const{
    int res = this->nrows;
    return res;
}
int PuzzleMatrix::getNcols() const{
    int res = this->ncols;
    return res;
}



/*
 * Update the constraints of a given neighbour, according to a specific edge of the piece that was
 * placed near that neighbour.
 */
void PuzzleMatrix::UpdateConstraintsOfNeighbour(PuzzlePiece* piece, Rotate rotation, Edge pieceEdgeToUpdateBy, Edge neighbourEdgeToUpdate,
                                                int neighbourRow, int neighbourCol) {
    return; //TODO: is this fun really not needed?
    switch (piece->getConstraint(pieceEdgeToUpdateBy, rotation)) {
        case STRAIGHT:
            matrix[neighbourRow][neighbourCol].constraints[neighbourEdgeToUpdate] = STRAIGHT;
            break;
        case MALE:
            matrix[neighbourRow][neighbourCol].constraints[neighbourEdgeToUpdate] = FEMALE;
            break;
        case FEMALE:
            matrix[neighbourRow][neighbourCol].constraints[neighbourEdgeToUpdate] = MALE;
            break;
        case NONE:
            break;
    }
}



/*
 * assign a given piece to a cell in the PuzzleMatrix, and updates the constraints of the neighbouring cells accordingly
 * assumes piece can be legally assigned there.
 */
void PuzzleMatrix::assignPieceToCell(PuzzlePiece* piece, Rotate rotation, int row, int col){
    matrix[row][col].piece = piece; //TODO: maybe no need to actually keep Piece in matrix? just id?
    matrix[row][col].rotation = rotation;
    //Update constraints of neighbours:
    if (row > 0){//cell has a neighbour above
        UpdateConstraintsOfNeighbour(piece, rotation, TOP, BOTTOM, row-1, col); //TODO: maybe send piece here byRef?
        }
    if (row < nrows - 1){//cell has a neighbour below
        UpdateConstraintsOfNeighbour(piece, rotation,  BOTTOM, TOP, row+1, col);
    }
    if (col > 0){//cell has a neighbour on the left
        UpdateConstraintsOfNeighbour(piece, rotation, LEFT, RIGHT, row, col-1);
    }
    if (col < ncols - 1){//cell has a neighbour on the right
        UpdateConstraintsOfNeighbour(piece, rotation, RIGHT, LEFT, row, col+1);
    }

    updateRequiredCounters(piece, rotation, row, col);
}


void PuzzleMatrix::updateRequiredCounters(PuzzlePiece* piece, Rotate rotation, int row, int col){
    //If piece was placed at edge of matrix, update requiredStraights:
    if (row == 0 || row == this->nrows -1) { this->requiredCounters[STRAIGHT]--; }
    if (col == 0 || col == this->ncols -1) { this->requiredCounters[STRAIGHT]--; }

    if (row > 0) {//Given cell has a neighbour above
        if (matrix[row - 1][col].piece == nullptr) // The neighbour is vacant
            this->requiredCounters[piece->getOppositeConstraint(TOP)]++;
        else // The neighbour is NOT vacant
            this->requiredCounters[piece->getConstraint(TOP, rotation)]--;
    }
    if (row < nrows - 1){//Given cell has a neighbour below
        if (matrix[row+1][col].piece == nullptr) // The neighbour is vacant
            this->requiredCounters[piece->getOppositeConstraint(BOTTOM)]++;
        else // The neighbour is NOT vacant
            this->requiredCounters[piece->getConstraint(BOTTOM, rotation)]--;
    }
    if (col > 0){//Given cell has a neighbour on the left
        if (matrix[row][col-1].piece == nullptr) // The neighbour is vacant
            this->requiredCounters[piece->getOppositeConstraint(LEFT)]++;
        else // The neighbour is NOT vacant
            this->requiredCounters[piece->getConstraint(LEFT, rotation)]--;
    }
    if (col < ncols - 1 ){//Given cell has a neighbour on the right
        if (matrix[row][col+1].piece == nullptr) // The neighbour is vacant
            this->requiredCounters[piece->getOppositeConstraint(RIGHT)]++;
        else // The neighbour is NOT vacant
            this->requiredCounters[piece->getConstraint(RIGHT, rotation)]--;
    }


}



/*
 * check if given PuzzlePiece can be fitted to PuzzleMatrix[row][col], according to constraints of that cell.
 * if piece does not fit or cell is full (i.e it's piece pointer != NULL) - returns False, else True.
 * if every non-NULL constraint matches the edge of the piece --> fits.
 */
bool PuzzleMatrix:: isFit(PuzzlePiece* piece, int row, int col){
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
                if (piece->getConstraint((Edge) i) != FEMALE){
                    return false;
                }
                break;
            case (MALE):
                if (piece->getConstraint((Edge) i) != MALE){
                    return false;
                }
                break;
            case (STRAIGHT):
                if (piece->getConstraint((Edge) i) != STRAIGHT){
                    return false;
                }
                break;
        }
    }
    return true;
}


void PuzzleMatrix::toFile(string path) {
    ofstream fout(path);
    for (int i=0; i<nrows; i++){

        for (int j=0; j<ncols; j++){
            if (matrix[i][j].piece == NULL) {
                fout << "NA" << " ";
                continue;
            }
            fout << matrix[i][j].piece->getId();
            if (matrix[i][j].rotation != Rotate::NO) {
                fout << " [" << matrix[i][j].rotation << "]";
            }
            if (j < (ncols-1)){
                fout<< " ";
            }
        }
        if (i < nrows -1 ){fout << endl;}
    }
    fout<<std::endl;
    fout.close();
}

void PuzzleMatrix::print() {
    for (int i=0; i<nrows; i++){

        for (int j=0; j<ncols; j++){
            if (matrix[i][j].piece == NULL) {
                cout << "N" << " ";
                continue;
            }
            cout << matrix[i][j].piece->getId() << " ";
        }
        cout << endl;
    }
}


string PuzzleMatrix::toString(){
    string s;
    for (int i=0; i<nrows; i++){

        for (int j=0; j<ncols; j++){
            if (matrix[i][j].piece == NULL) {
                s.append("N");
                s.append(" ");
                continue;
            }
            s.append(std::to_string(matrix[i][j].piece->getId()));
            s.append(" ");
        }
        s.append("\n");
    }
    return s;
}



//TODO: not applicable for rotations, remove if no other need for this func (replaced by getConstraintsOfCell)
void PuzzleMatrix::constraintsOfCell(int row, int col, int* res) {
    //{LEFT = 0, TOP = 1, RIGHT = 2, BOTTOM = 3, LAST};
    if (col == 0) {
        res[LEFT] = STRAIGHT;
    } else {
        res[LEFT] = matrix[row][col - 1].piece->getConstraint(RIGHT, matrix[row][col-1].rotation);
        // TODO: above hack can be more efficient if we don't call "getConstraint(RIGHT, matrix[row][col-1].rotation)" in the non-rotatable case
    }
    if (col == (ncols - 1)) {
        res[RIGHT] = STRAIGHT;
    }
    else {
       // res[RIGHT] = matrix[row][col + 1].piece->getConstraint(LEFT, matrix[row][col+1].rotation);
        // TODO: above hack can be more efficient if we don't call "getConstraint(RIGHT, matrix[row][col-1].rotation)" in the non-rotatable case
    }
    if (row == 0) {
        res[TOP] = STRAIGHT;
    } else {
        res[TOP] = matrix[row - 1][col].piece->getConstraint(BOTTOM, matrix[row-1][col].rotation);
        // TODO: above hack can be more efficient if we don't call "getConstraint(RIGHT, matrix[row][col-1].rotation)" in the non-rotatable case
    }
    if (row == (nrows - 1)) {
        res[BOTTOM] = STRAIGHT;
    } else {
      //  res[BOTTOM] = matrix[row+1][col].piece->getConstraint(TOP, matrix[row+1][col].rotation);
        // TODO: above hack can be more efficient if we don't call "getConstraint(RIGHT, matrix[row][col-1].rotation)" in the non-rotatable case
    }
}

void PuzzleMatrix::getConstraintsOfCell(int row, int col, int* res){
    for (int i=0; i<4; i++) { res[i] = (*this).matrix[row][col].constraints.at(i); }
}
