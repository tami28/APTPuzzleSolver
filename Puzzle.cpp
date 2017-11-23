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
	while(getline(fin,line)){
		try {
			curr = PuzzlePiece(line);
			int i = addPiece(curr);
			if (i == ILLEGAL_PIECE){ //TODO: Yoav: I think this is no longer needed (piece c'tor will throw the relevant error).
				continue; //TODO
			}
			totalSum +=i;
		}
		catch (Error e) {
			if (e.getErrorType() == _WRONG_PIECE_ID){ //collect all wring IDs' as they need to create one error.
				wrongIDs.push_back(e.getIntInfo());
			}
			else {
				(*ErrorList::getErrorList()).add(e);
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


	fin.close();
	if(0 != totalSum){
		(*ErrorList::getErrorList()).add(Error(SUM_EDGES_NOT_ZERO));
	}
	for (int i=TL; i!= LAST_C; i++){
		if(0 == _corners[i]){
			(*ErrorList::getErrorList()).add(Error(MISSING_CORNER, i));
		}
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
		_corners[TL]++;
	}
	if ((0== piece.getConstraint(TOP)) && (0== piece.getConstraint(RIGHT))){
		_corners[TR]++;
	}
	if ((0== piece.getConstraint(BOTTOM)) && (0== piece.getConstraint(LEFT))){
		_corners[BL]++;
	}
	if ((0== piece.getConstraint(BOTTOM)) && (0== piece.getConstraint(RIGHT))){
		_corners[BR]++;
	}
	return sum;

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