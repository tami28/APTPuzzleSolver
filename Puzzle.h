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
	map<Corners ,set<int>> _corners;
	int _straightEdges[4] = {0};
	//TODO: represent here possible corners?
//functions:
public:
	//constructors:
	Puzzle();
	Puzzle(string fileName);

	//Dt'r: We didn't assign anything with new, nor de we have pointers therefore we don't need to implement a Dt'r.

	void buildPuzzleFromFile(const string& fileName);
	PuzzlePiece* getPieceAt(int i);
    int getSize();
	int getMaxHeight();
	int getMaxWidth();
	bool checkForMissingPieces(vector<int>& ids, vector<int> &idsFromFile);
	void getPossibleSizes(vector<pair<int,int>> & result);

private:
	void parseFirstLine(string line);
	int addPiece(PuzzlePiece& piece);
	void checkCorners();
	void checkStraightEdges();

};


#endif /* PUZZLE_H_ */
