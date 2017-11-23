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



class Puzzle{
//members:
private:
	vector<PuzzlePiece> _pieces;
	int _size = 0;
	int _corners[4] = {0}; //<TL><TR><BL><BR>
	int _straightEdges[4] = {0};
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
	int getMaxHeight();
	int getMaxWidth();
	bool checkForMissingPieces(vector<int>& ids, vector<int> &idsFromFile);

private:
	void parseFirstLine(string line);
	int addPiece(PuzzlePiece& piece);

};


#endif /* PUZZLE_H_ */
