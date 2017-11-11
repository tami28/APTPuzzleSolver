/*
 * Puzzle.h
 *
 *  Created on: 10 бреб„ 2017
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

/*
 * Class Puzzle:
constructor(file)?
Check initial valid puzzle (from reading to array)
Check valid pieces puzzle - corners, sum edges wrong # of straight edges..
 *
 */

class Puzzle{
//members:
private:
	vector<PuzzlePiece> _pieces;
	vector<Error> _errors;
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

	Error buildPuzzleFromFile(const string& fileName);

private:
	Error sumEdges();
	Error missingCorner();
	Error parseFirstLine(string line);

};


#endif /* PUZZLE_H_ */
