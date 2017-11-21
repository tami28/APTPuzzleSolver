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
    this->frontierCells = other.frontierCells;
    this->matrix = other.matrix;
}

const int PuzzleMatrix::getNrows() const{
    const int res = this->nrows;
    return res;
}
const int PuzzleMatrix::getNcols() const{
    const int res = this->ncols;
    return res;
}



/*
 * Update the constraints of a given neighbour, according to a specific edge of the piece that was
 * placed near that neighbour.
 */
void PuzzleMatrix::UpdateConstraintsOfNeighbour(PuzzlePiece* piece,Edge pieceEdgeToUpdateBy, Edge neighbourEdgeToUpdate,
                                                int neighbourRow, int neighbourCol) {
    switch (piece->getConstraint(pieceEdgeToUpdateBy)) {
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



    updateRequiredCounters(piece, row, col);
    updateFrontiers(row, col);
}


void PuzzleMatrix::updateRequiredCounters(PuzzlePiece* piece, int row, int col){
    //If piece was placed at edge of matrix, update requiredStraights:
    if (row == 0 || row == this->nrows -1) { this->requiredCounters[STRAIGHT]--; }
    if (col == 0 || col == this->ncols -1) { this->requiredCounters[STRAIGHT]--; }

    if (row > 0) {//Given cell has a neighbour above
        if (matrix[row - 1][col].piece == nullptr) // The neighbour is vacant
            this->requiredCounters[piece->getOppositeConstraint(TOP)]++;
        else // The neighbour is NOT vacant
            this->requiredCounters[piece->getConstraint(TOP)]--;
    }
    if (row < nrows - 1){//Given cell has a neighbour below
        if (matrix[row+1][col].piece == nullptr) // The neighbour is vacant
            this->requiredCounters[piece->getOppositeConstraint(BOTTOM)]++;
        else // The neighbour is NOT vacant
            this->requiredCounters[piece->getConstraint(BOTTOM)]--;
    }
    if (col > 0){//Given cell has a neighbour on the left
        if (matrix[row][col-1].piece == nullptr) // The neighbour is vacant
            this->requiredCounters[piece->getOppositeConstraint(LEFT)]++;
        else // The neighbour is NOT vacant
            this->requiredCounters[piece->getConstraint(LEFT)]--;
    }
    if (col < ncols - 1 ){//Given cell has a neighbour on the right
        if (matrix[row][col+1].piece == nullptr) // The neighbour is vacant
            this->requiredCounters[piece->getOppositeConstraint(RIGHT)]++;
        else // The neighbour is NOT vacant
            this->requiredCounters[piece->getConstraint(RIGHT)]--;
    }


}




/*
 * update list of frontier cells after insertion of a piece to the matrix.
 */
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
 * returns iterator to beginning of frontierCells set.
 */
std::set<pair<int,int>>::iterator PuzzleMatrix::getFrontierCellsIterator() const{
    return this->frontierCells.begin();
};





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
            fout << matrix[i][j].piece->getId() << " ";
        }
        if (i < nrows -1 ){fout << endl;}
    }
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

//TODO: is it ok to return a string this way? no need to allocate it dynamically?
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

string PuzzleMatrix::encode(){
    string s;
    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            if (matrix[i][j].piece == NULL) {
                s.append("N");
                s.append("|");
                continue;
            }
            s.append(std::to_string(matrix[i][j].piece->getEncoding()));
            s.append("|");
        }
    }
    return s;
}