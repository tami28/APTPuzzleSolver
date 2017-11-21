/*
 * Puzzle.h
 *
 *  Created on: 10 november 2017
 *      Author: Tami
 */

#ifndef PUZZLE_H_
#define PUZZLE_H_

#include "PuzzlePiece.h"
#include "Exceptions.h"
#include <vector>
#include <string.h>
#include <map>
using namespace std;

//TODO: how to output the errors in the needed order?

#define ILLEGAL_PIECE -5


inline int edgeConstraintToInt(Edge e, Constraints c){return e*10+c;};

class Puzzle{
//members:
private:
	vector<PuzzlePiece> _pieces;
	int _size = 0;
	int _corners[4] = {0}; //<TL><TR><BL><BR>
	std::map<int, std::vector<PuzzlePiece>> _constraintEdgsMaps;
	//TODO: represent here possible corners?
//functions:
public:
	//constructors:
	Puzzle();
	Puzzle(string fileName);

	//destructors:
	//TODO!

	void buildPuzzleFromFile(const string& fileName);
	PuzzlePiece* getPieceAt(int i);
    int getSize();
    std::map<int, std::vector<PuzzlePiece>>* getEdgeConstraintsMap();

private:
	void sumEdges();
	void missingCorner();
	void parseFirstLine(string line);
	int addPiece(PuzzlePiece& piece);

};


#endif /* PUZZLE_H_ */
