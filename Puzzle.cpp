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


Error Puzzle::buildPuzzleFromFile(const string& fileName){
	ifstream fin(fileName);
	string line;
	Error err;
	getline(fin, line);
	//assumption: if couldn't find properly tthe size the size will be set to zero, meaning all puzzle pieces will be illegal!
	if (SUCCESS != (err = parseFirstLine(line))){
		_errors.push_back(err);
	}
	while(getline(fin,line)){

	}


}

