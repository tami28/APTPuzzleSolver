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
    _requiredCounters[STRAIGHT] = 2*nrows + 2*ncols;
    _requiredCounters[MALE] = 0;
    _requiredCounters[FEMALE] = 0;

    //Initialize number of outer-frame straight edges needed for this puzzleMatrix:
    _requieredFrameConstraints[outerFrameConstraints::LEFT_STRAIGHT] = nrows;
    _requieredFrameConstraints[outerFrameConstraints::TOP_STRAIGHT] = ncols;
    _requieredFrameConstraints[outerFrameConstraints::RIGHT_STAIGHT] = nrows;
    _requieredFrameConstraints[outerFrameConstraints::BOTTOM_STRAIGHT] = ncols;
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
    this->_requiredCounters = other._requiredCounters;
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
 * assign a given piece to a cell in the PuzzleMatrix, and updates the constraints of the neighbouring cells accordingly
 * assumes piece can be legally assigned there.
 */
void PuzzleMatrix::assignPieceToCell(PuzzlePiece* piece, Rotate rotation, int row, int col){
    matrix[row][col].piece = piece;
    matrix[row][col].rotation = rotation;

    updateRequiredCounters(piece, rotation, row, col);
}


void PuzzleMatrix::updateRequiredCounters(PuzzlePiece* piece, Rotate rotation, int row, int col){
    //If piece was placed at edge of matrix, update requiredStraights:
    if (row == 0 || row == this->nrows -1) { this->_requiredCounters[STRAIGHT]--; }
    if (col == 0 || col == this->ncols -1) { this->_requiredCounters[STRAIGHT]--; }

    if (row > 0) {//Given cell has a neighbour above
        if (matrix[row - 1][col].piece == nullptr) // The neighbour is vacant
            this->_requiredCounters[piece->getOppositeConstraint(TOP)]++;
        else // The neighbour is NOT vacant
            this->_requiredCounters[piece->getConstraint(TOP, rotation)]--;
    }
    if (row < nrows - 1){//Given cell has a neighbour below
        if (matrix[row+1][col].piece == nullptr) // The neighbour is vacant
            this->_requiredCounters[piece->getOppositeConstraint(BOTTOM)]++;
        else // The neighbour is NOT vacant
            this->_requiredCounters[piece->getConstraint(BOTTOM, rotation)]--;
    }
    if (col > 0){//Given cell has a neighbour on the left
        if (matrix[row][col-1].piece == nullptr) // The neighbour is vacant
            this->_requiredCounters[piece->getOppositeConstraint(LEFT)]++;
        else // The neighbour is NOT vacant
            this->_requiredCounters[piece->getConstraint(LEFT, rotation)]--;
    }
    if (col < ncols - 1 ){//Given cell has a neighbour on the right
        if (matrix[row][col+1].piece == nullptr) // The neighbour is vacant
            this->_requiredCounters[piece->getOppositeConstraint(RIGHT)]++;
        else // The neighbour is NOT vacant
            this->_requiredCounters[piece->getConstraint(RIGHT, rotation)]--;
    }
    updateRequiredFrameCounters(row, col);
}

/*
 * Update number of required leftStraight, bottomStraight, etc. (i.e, frame constraints) after assigning a piece at (row,col).
 * Assumes piece was validly assigned.
 */
void PuzzleMatrix::updateRequiredFrameCounters(int row, int col){
    if (row == 0) { _requieredFrameConstraints[outerFrameConstraints::TOP_STRAIGHT]--; }
    if (row == nrows - 1) { _requieredFrameConstraints[outerFrameConstraints::BOTTOM_STRAIGHT]--; }
    if (col == 0) { _requieredFrameConstraints[outerFrameConstraints::LEFT_STRAIGHT]--; }
    if (col == ncols - 1) { _requieredFrameConstraints[outerFrameConstraints::RIGHT_STAIGHT]--; }
}

/*
 * Return the number of required outer frame constraints (topRight, bottomLeft, etc.) for a given outerFrameConstraint.
 */
int PuzzleMatrix::getOuterFrameRequirementAt(outerFrameConstraints c){
    return _requieredFrameConstraints[c];
}


bool PuzzleMatrix::isCornerRequired(Corners c){
    switch (c){
        case Corners::TL:
            return (matrix[0][0].piece == nullptr);
        case Corners::TR:
            return (matrix[0][getNcols()-1].piece == nullptr);
        case Corners::BR:
            return (matrix[getNrows()-1][getNcols()-1].piece == nullptr);
        case Corners::BL:
            return (matrix[getNrows()-1][0].piece == nullptr);
        default:
            return false;
    }
}

/*
 * check if given PuzzlePiece can be fitted to PuzzleMatrix[row][col], according to constraints of that cell.
 * if piece does not fit or cell is full (i.e it's piece pointer != NULL) - returns False, else True.
 * if every non-NULL constraint matches the edge of the piece --> fits.
 */
bool PuzzleMatrix:: isFit(PuzzlePiece* piece, int row, int col){
    if (matrix[row][col].piece != nullptr){
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
    if (fout.fail()) {
        cout << "Could not write solution to file\n";
    }
    for (int i=0; i<nrows; i++){

        for (int j=0; j<ncols; j++){
            if (matrix[i][j].piece == nullptr) {
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
            if (matrix[i][j].piece == nullptr) {
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
            if (matrix[i][j].piece == nullptr) {
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



void PuzzleMatrix::constraintsOfCell(int row, int col, int* res, StepType step) {
    switch(step){
        case COL:
        case ROW:
            constraintsOfCellByRow(row, col, res);
            return;
        case FRAME:
            constraintsOfCellByFrame(row,col, res);
    }

}


void PuzzleMatrix::constraintsOfCellByRow (int row, int col, int * res) {
    //{LEFT = 0, TOP = 1, RIGHT = 2, BOTTOM = 3, LAST};
    if (col == 0) {
        res[LEFT] = STRAIGHT;
    } else {
        res[LEFT] = matrix[row][col - 1].piece->getConstraint(RIGHT, matrix[row][col-1].rotation);
    }
    if (col == (ncols - 1)) {
        res[RIGHT] = STRAIGHT;
    }
    if (row == 0) {
        res[TOP] = STRAIGHT;
    } else {
        res[TOP] = matrix[row - 1][col].piece->getConstraint(BOTTOM, matrix[row-1][col].rotation);
    }
    if (row == (nrows - 1)) {
        res[BOTTOM] = STRAIGHT;
    }
}
void PuzzleMatrix::constraintsOfCellByFrame(int row, int col, int *res) {
    bool frame = false;
    if (row == 0){
        res[TOP] = STRAIGHT;
        if (col==0){
            res[LEFT] = STRAIGHT;
        } else{
            res[LEFT] = matrix[row][col -1].piece->getConstraint(RIGHT, matrix[row][col-1].rotation);
        }
        frame = true;
    }
    if (col == ncols-1){
        res[RIGHT] = STRAIGHT;
        if(row>0){
            res[TOP] = matrix[row -1][col].piece->getConstraint(BOTTOM, matrix[row-1][col].rotation);
        }
        frame = true;
    }
    if(row == nrows-1){
        res[BOTTOM] = STRAIGHT;
        if (col < ncols-1){
            res[RIGHT] = matrix[row][col+1].piece->getConstraint(LEFT, matrix[row][col+1].rotation);
        }
        frame = true;
    }
    if(col == 0){
        res[LEFT] = STRAIGHT;
        if (row < nrows-1 && row > 0){
            res[BOTTOM] = matrix[row+1][col].piece->getConstraint(TOP, matrix[row+1][col].rotation);
        }
        if (row == 1){
            res[TOP] = matrix[row-1][col].piece->getConstraint(BOTTOM, matrix[row-1][col].rotation);
        }
        frame = true;
    }
    if (frame){
        return;
    }
    //FINISHED FRAME !!!!
    res[TOP] = matrix[row -1][col].piece->getConstraint(BOTTOM, matrix[row-1][col].rotation);
    res[LEFT] = matrix[row][col -1].piece->getConstraint(RIGHT, matrix[row][col-1].rotation);
    if (col == ncols-2){
        res[RIGHT] = matrix[row][col +1].piece->getConstraint(LEFT, matrix[row][col+1].rotation);
    }
    if (row == nrows-2){
        res[BOTTOM] = matrix[row +1][col].piece->getConstraint(TOP, matrix[row+1][col].rotation);
    }
}

std::map<Constraints , int> PuzzleMatrix::getRequiredCounters() {
    return this->_requiredCounters;
}

std::map<outerFrameConstraints, int> PuzzleMatrix::getRequiredFrameConstraints(){
    return this->_requieredFrameConstraints;
}

void  PuzzleMatrix::setRequiredCounters(std::map<Constraints , int> requiredCounters){
    this->_requiredCounters = requiredCounters;
}
void  PuzzleMatrix::setRequiredFrameConstraints(std::map<outerFrameConstraints , int> requiredFrameCounters){
    this->_requieredFrameConstraints = requiredFrameCounters;
}