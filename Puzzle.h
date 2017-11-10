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
#include <string>
using namespace std;


/*
 * Class Puzzle:
constructor(file)?
Check initial valid puzzle (from reading to array)
Check valid pieces puzzle - corners, sum edges wrong # of straight edges..
 *
 */

class Puzzle{
//members:
	vector<PuzzlePiece> pieces;
	int size;
	int numEdges[4]; //for each one of the sides..
	//TODO: represnt here popssible corners?
//functions:
public:
	//constructors:
	Puzzle(){};
	Puzzle(string fileName){};

	//destructors:
	//TODO!

	Error buildPuzzleFromFile(string fileName);

private:
	Error sumEdges();
	Error missingCorner();
};


#endif /* PUZZLE_H_ */
