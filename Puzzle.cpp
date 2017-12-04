/*

 * Puzzle.cpp
 *
 *  Created on: 11 nov 2017
 *      Author: Tami
 */
#include <string>
#include "Puzzle.h"
#include <sstream>
#include <iterator>
//#include "solver.h"
#include "PuzzleMatrix.h"
#include <numeric>

int numPieces; //initialization of global variable.

//constructors:

/*
 * initialize basic Puzzle
 */
Puzzle::Puzzle() {}

/*
 * initialize Puzzle from file:
 */
Puzzle::Puzzle(string fileName){

	Puzzle::buildPuzzleFromFile(fileName);
}



void Puzzle::buildPuzzleFromFile(const std::string& fileName){
	ifstream fin(fileName);
	std::string line;
	PuzzlePiece curr;
	getline(fin, line);
	//assumption: if couldn't find properly tthe size the size will be set to zero, meaning all puzzle pieces will be illegal!

	parseFirstLine(line);
	_pieces.resize(_size);
	numPieces = _size;
	//Read all lines, assuming that the piece constructor adds to errs if there's  a problem
	std::vector<int> wrongIDs;
	std::vector<int> idsFromFile; //holds all integer IDs seen in file (for detecting missing IDs).
	while(getline(fin,line)){
		try {
			curr = PuzzlePiece(line, idsFromFile);
			int i = addPiece(curr);
//			if (i == ILLEGAL_PIECE){

//				continue;
//			}
			_totalSum +=i;
		}
		catch (vector<Error> eVec) {
            for (Error e: eVec) {
                if (e.getErrorType() == _WRONG_PIECE_ID) { //collect all wrong ID's as they need to create one error.
                    wrongIDs.push_back(e.getIntInfo());
                } else {
                    (*ErrorList::getErrorList()).add(e);
                }
            }
                continue;

		};
	}
	//Write to errors list an error for all wrong (numeric) IDs:
	if (!wrongIDs.empty()) {
		stringstream ss;
		copy(wrongIDs.begin(), wrongIDs.end(), ostream_iterator<int>(ss, ", "));
		string infoStr = ss.str().substr(0, (ss.str()).length() - 2);
		(*ErrorList::getErrorList()).add(Error(WRONG_PIECE_IDS, infoStr));
	}

	//Check for missing pieces:
	vector<int> missingPieces;
	if (!checkForMissingPieces(idsFromFile, missingPieces)){
		stringstream ss;
		copy(missingPieces.begin(), missingPieces.end(), ostream_iterator<int>(ss, ", "));
		string infoStr = ss.str().substr(0, (ss.str()).length() - 2);
		(*ErrorList::getErrorList()).add(Error(MISSING_PIECES, infoStr));
	}
	fin.close();

	//Insert all pieces to the constraintsTable:
	for (auto p : _pieces){
		constraintsTable.insertPiece(p);
	}



}


void Puzzle::parseFirstLine(std::string line){
	std::string num;
	std::size_t const foundS = line.find("NumElements");
	std::size_t const foundEq = line.find("=", foundS+1);
	//Make sure format is ok:
	if (foundS ==std::string::npos || foundEq == std::string::npos){
		(*ErrorList::getErrorList()).add(Error(WRONG_FIRST_LINE_FORMAT, line));
	}
	//Read the number:
	std::size_t const n = line.find_first_of("0123456789");
	if (n != std::string::npos)
	{
	  std::size_t const m = line.find_first_not_of("0123456789", n);
	  num = line.substr(n, m != std::string::npos ? m-n : m);
	  _size = atoi(num.c_str());
	}

}

/*
 * get (by ref) the piece at index i in the puzzle.
 */
PuzzlePiece* Puzzle::getPieceAt(int i){
	return &(_pieces[i-1]);
}


bool Puzzle::checkForMissingPieces(vector<int> &idsFromFile, vector<int> &ids){
	vector<int> expected_ids(numPieces);
	std::iota(expected_ids.begin(), expected_ids.end(), 1);
	for (int id : idsFromFile){
		expected_ids.erase( std::remove(expected_ids.begin(), expected_ids.end(), id ), expected_ids.end() );
	}
	if (expected_ids.empty()){
		return true;
	}
	std::sort(expected_ids.begin(), expected_ids.end());
	ids = expected_ids;
	return false;

}


int Puzzle::addPiece(PuzzlePiece& piece) {
	int sum =0;
	_pieces[piece.getId() -1] = piece;
	for (int edge = LEFT; edge != LAST; ++edge){ //TODO
		sum += piece.getConstraint((Edge) edge);
        if (piece.getConstraint((Edge) edge) == STRAIGHT){
            _straightEdges[edge]++;
        }
	}

	//check corners:
	//<TL><TR><BL><BR>
	if ((0== piece.getConstraint(TOP)) && (0== piece.getConstraint(LEFT))){
		_corners[TL].insert(piece.getId());
	}
	if ((0== piece.getConstraint(TOP)) && (0== piece.getConstraint(RIGHT))){
		_corners[TR].insert(piece.getId());
	}
	if ((0== piece.getConstraint(BOTTOM)) && (0== piece.getConstraint(LEFT))){
		_corners[BL].insert(piece.getId());
	}
	if ((0== piece.getConstraint(BOTTOM)) && (0== piece.getConstraint(RIGHT))){
		_corners[BR].insert(piece.getId());
	}
	return sum;

}


/*
 * check that there are enough sufficient corner pieces to solve puzzle, and report error if not
 * according to: http://moodle.tau.ac.il/mod/forum/discuss.php?d=10775#p16816
 * in case there's a single-row/single-column solution for the puzzle, we skip the corners check (according to
 * http://moodle.tau.ac.il/mod/forum/discuss.php?d=10775  we don't have to output a special message for missing
 * corners in the single-row / single-col case).
 */
void Puzzle::checkCorners(){

//    Solver solver = Solver(*this);
//    if (isPrime(_size) || _size == 1 || solver.hasSingleRowColSolution()) {
//        return; //See doc!
//    }

    string errStr = "";
    int pieceUsedForCurrentCorner;
    for (int corner = TL ; corner <= BR ; corner++){
        if (_corners[(Corners) corner].empty()){
            switch (corner){
                case 0:
                    errStr.append("<TL>");
                    break;
                case 1:
                    errStr.append("<TR>");
                    break;
                case 2:
                    errStr.append("<BL>");
                    break;
                case 3:
                    errStr.append("<BR>");
                    break;
            }
        }
        else{
            pieceUsedForCurrentCorner = *(_corners[(Corners) corner].begin());
            for (int corner2 = TL ; corner2 <= BR ; corner2++){
                _corners[(Corners) corner2].erase(pieceUsedForCurrentCorner);
            }
        }
    }
    if (errStr != "") {
        (*ErrorList::getErrorList()).add(Error(MISSING_CORNER, errStr));
    }
}


void Puzzle::getPossibleSizes(vector<pair<int,int>> & result){
	int puzzleSize = _size;
	int sqr = (int) sqrt(puzzleSize) + 1;
	for (int i = 1; i < sqr; i++) {
		if (puzzleSize % i == 0 && i<=max(getMaxWidth(), getMaxHeight())&& (puzzleSize/i)<=max(getMaxWidth(), getMaxHeight())) {
			result.push_back(pair<int, int>(i, puzzleSize / i));
			if (i != puzzleSize / i) {
				result.push_back(pair<int, int>(puzzleSize / i,
												i));
                //TODO: currently we don't rotate pieces, so we need to check both n*m and m*n for m!=n (o/w will not find solution)
			}
		}
	}
}

/*
 * check that there are sufficient straight edges to solve puzzle, and report error if not.
 * There are sufficient straight edges if there exists nrows, ncols such that:
 * 	 	1. nrows*ncols == numPieces
 * 		2. numStraightEdges >= 2*nrows + 2*ncols
 * 		3. (numStraightEdges - 2*nrows + 2*ncols) % 2 == 0
 */
void Puzzle::checkStraightEdges(){
	vector<pair<int,int>> possibleSizes;
	getPossibleSizes(possibleSizes);
	int nrows, ncols;
	int numStraightEdges = _straightEdges[LEFT] + _straightEdges[RIGHT] + _straightEdges[TOP] + _straightEdges[BOTTOM];
	bool sufficient = false;
	for (auto size : possibleSizes) {
		nrows = size.first;
		ncols = size.second;
		if ( (numStraightEdges >= 2*nrows + 2*ncols)
			 && ((numStraightEdges - (2*nrows + 2*ncols)) % 2 == 0) ){
			sufficient = true;
			break;
		}
	}

	if (!sufficient){
		(*ErrorList::getErrorList()).add(Error(WRONG_NUM_STRAIGHT_EDGES));
	}

}


int Puzzle::getSize(){
	return this->_size;
}

int Puzzle::getMaxHeight() {
    return min(_straightEdges[LEFT], _straightEdges[RIGHT]);
}

int Puzzle::getMaxWidth() {
    return min(_straightEdges[TOP], _straightEdges[BOTTOM]);
}


bool Puzzle::isPrime(int n){
    if (n < 2) { return false; }
    for (int i = 2; i <= int(floor(sqrt(n))); i ++) {
        if (n % i == 0) { return false; };
    }
    return true;
}

void Puzzle::selAllPiecesValid(){
	for (PuzzlePiece& p : _pieces){
		//p.setIsValid(true);
		p.isValid = true;
	}
}