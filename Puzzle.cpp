/*

 * Puzzle.cpp
 *
 *  Created on: 11 nov 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "const.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>


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
	ErrorList* errList = ErrorList::getErrorList();
	PuzzlePiece curr;
	getline(fin, line);
	int totalSum = 0;
	//assumption: if couldn't find properly tthe size the size will be set to zero, meaning all puzzle pieces will be illegal!

	parseFirstLine(line);
	_pieces.resize(_size);
	numPieces = _size;
	//TODO: what if failed reading size? fix this!

	//Read all lines, assuming that the piece constructor adds to errs if there's  a problem
	std::vector<int> wrongIDs;
	std::vector<int> idsFromFile; //holds all integer IDs seen in file (for detecting missing IDs).
	while(getline(fin,line)){
		try {
			curr = PuzzlePiece(line, idsFromFile);
			int i = addPiece(curr);
			if (i == ILLEGAL_PIECE){ //TODO: Yoav: I think this is no longer needed (piece c'tor will throw the relevant error).
				continue; //TODO
			}
			//idsFromFile.push_back(curr.getId());
			totalSum +=i;
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

	//TODO: should the following errors be reported even if one of the above errors had occurred?

	//Report missing corner error:
	checkCorners();
	//Report sum-not-zero error:
    if(0 != totalSum){
		(*ErrorList::getErrorList()).add(Error(SUM_EDGES_NOT_ZERO));
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


int Puzzle::addPiece(PuzzlePiece &piece) {
	if (0 >= piece.getId()){
		return ILLEGAL_PIECE; //TODO: Yoav: I think this is no longer needed (piece c'tor will throw the relevant error).
	}
	int sum =0;
	_pieces[piece.getId() -1] = piece;
	for (int edge = LEFT; edge != LAST; ++edge){
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

void Puzzle::checkCorners(){
	string errStr = "";
	if (_corners[TL].empty()) { errStr.append("<TL>"); }
	if (_corners[TR].empty()) { errStr.append("<TR>"); }
	if (_corners[BL].empty()) { errStr.append("<BL>"); }
	if (_corners[BR].empty()) { errStr.append("<BR>"); }
	if (errStr != "") {
		(*ErrorList::getErrorList()).add(Error(MISSING_CORNER, errStr));
		return;
	}

    for (auto tl_candidate : _corners[TL]){
        for (auto tr_candidate : _corners[TR]){
            for (auto bl_candidate : _corners[BL]){
                for (auto br_candidate : _corners[BR]){
                    if (set<int>({tl_candidate, bl_candidate, tr_candidate, br_candidate}).size() >= 4) {
						return; // Got here --> all 4 corners can be covered with 4 different pieces
                    }
                }
            }
        }
    }
	//TODO: get an answer for what to print for this err
	(*ErrorList::getErrorList()).add(Error(CORNERS_CANT_BE_COVERED));

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