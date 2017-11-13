/*
 * Puzzle.h
 *
 *  Created on: 10 ����� 2017
 *      Author: Tami
 */

#ifndef PUZZLE_H_
#define PUZZLE_H_

#include "PuzzlePiece.h"
#include "Exceptions.h"
#include <vector>
#include <string.h>
using namespace std;

//TODO: how to output the errors in the needed order?

#define NUM_ELEMNTS = "NumElements"

class Puzzle{
//members:
private:
	vector<PuzzlePiece> _pieces;
	int _size;
	int _numEdges[4]; //for each one of the sides..
	//TODO: represent here possible corners?
//functions:
public:
	//constructors:
	Puzzle();
	Puzzle(string fileName);

	//destructors:
	//TODO!

	void buildPuzzleFromFile(const string& fileName);

private:
	Error sumEdges();
	Error missingCorner();
	Error parseFirstLine(string line);

};


#endif /* PUZZLE_H_ */
