/*

 * Puzzle.cpp
 *
 *  Created on: 11 бреб„ 2017
 *      Author: Tami
 */

#include "Puzzle.h"
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

	if (err!= NULL){
		errList.add(err);
	}
	_pieces.resize(_size);
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

Error Puzzle::parseFirstLine(string line){
	size_t find = line.find(NUM_ELEMNTS);
	if
}

/// global error vector
//global fatal error vector
//TODO: puzzle piece constructor return NULL if failed
