/*

 * Puzzle.cpp
 *
 *  Created on: 11 бреб„ 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "const.h"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
//constructors:

/*
 * initialize basic Puzzle
 */
Puzzle::Puzzle() {
	_size = 0;
	memset(_numEdges,0, sizeof(_numEdges));
}

/*
 * initialize Puzzle from file:
 */
Puzzle::Puzzle(string fileName){
	Puzzle::buildPuzzleFromFile(fileName);
}


Error Puzzle::sumEdges(){
	if(_numEdges[LEFT] != _numEdges[RIGHT] || _numEdges[UP] != _numEdges[DOWN]){
		return SUM_EDGES_NOT_ZERO;
	}
	return SUCCESS;
}


void Puzzle::buildPuzzleFromFile(const string& fileName){
	ifstream fin(fileName);
	string line;
	ErrorList errList = ErrorList::getErrorList();
	Error err;
	PuzzlePiece curr;
	getline(fin, line);
	//assumption: if couldn't find properly tthe size the size will be set to zero, meaning all puzzle pieces will be illegal!
	err = parseFirstLine(line);

	if (err!= nullptr){
		errList.add(err);
	}
	_pieces.resize(_size);
	numPieces = _size;
	//TODO: what if failed reading size? fix this!

	//Read all lines, assuming that the piece constructor adds to errs if there's  a problem
	while(getline(fin,line)){
		curr = PuzzlePiece(line);
		//assuming the PuzzlePiece constructor returns nullptr if failed completly
		if (curr == nullptr){
			continue;
		}
		if (curr.getId( )> _size){
			errList.add(Error(WRONG_PIECE_ID, curr.getId()));
			continue;
		}
		_pieces[curr.getId()] = curr;
	}
}

Error Puzzle::parseFirstLine(std::string line){
	ErrorList* errList = ErrorList::getErrorList();
	std::string num;
	std::size_t const foundS = line.find("NumElements");
	std::size_t const foundEq = line.find("="), foundS);
	//Make sure format is ok:
	if (foundS ==std::string::npos || foundEq == std::string::npos){
		(*errList).add(Error(WRONG_FORMET, line));
	}
	//Read the number:
	std::size_t const n = line.find_first_of("0123456789");
	if (n != std::string::npos)
	{
	  std::size_t const m = line.find_first_not_of("0123456789", n);
	  num = line.substr(n, m != std::string::npos ? m-n : m);
	  //_size = std::stoi(num, m-n);
  }

}

/// global error vector
//global fatal error vector
//TODO: puzzle piece constructor return NULL if failed
