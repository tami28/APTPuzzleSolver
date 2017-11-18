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


int numPieces; //initialization of global variable.

//constructors:

/*
 * initialize basic Puzzle
 */
Puzzle::Puzzle() {
}

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
	while(getline(fin,line)){
		try {
			curr = PuzzlePiece(line);
			int i = addPiece(curr);
			if (i == ILLEGAL_PIECE){
				continue; //TODO
			}
			totalSum +=i;
		}
		catch (Error e) {
			(*ErrorList::getErrorList()).add(e);
			continue;
		};
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
		(*ErrorList::getErrorList()).add(Error(WRONG_FORMET, line));
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
		return ILLEGAL_PIECE;
	}
	int sum =0;
	_pieces[piece.getId() -1] = piece;
	for (int edge = LEFT; edge != LAST; ++edge){
		if(0 == piece.getEdge((Edge)edge)){
			_numEdges[edge]++;

		}
		sum += piece.getEdge((Edge)edge);
	}

	//check corners:
	//<TL><TR><BL><BR>
	if ((0== piece.getEdge(TOP)) && (0==piece.getEdge(LEFT))){
		_corners[TL]++;
	}
	if ((0== piece.getEdge(TOP)) && (0==piece.getEdge(RIGHT))){
		_corners[TR]++;
	}
	if ((0== piece.getEdge(BOTTOM)) && (0==piece.getEdge(LEFT))){
		_corners[BL]++;
	}
	if ((0== piece.getEdge(BOTTOM)) && (0==piece.getEdge(RIGHT))){
		_corners[BR]++;
	}
	return sum;

}

int Puzzle::getSize(){
	return this->_size;
}