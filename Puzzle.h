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
#include "const.h"
#include "ConstraintsTable.h"
using namespace std;

#define ILLEGAL_PIECE -5



class Puzzle{

//members:
protected:
	vector<PuzzlePiece> _pieces;
	int _size = 0;
    int _totalSum = 0;
	map<Corners ,set<int>> _corners;
	int _straightEdges[4] = {0};
//functions:
public:
	//constructors:
	Puzzle();
	Puzzle(string fileName);
	ConstraintsTable constraintsTable;
	//Dt'r: We didn't assign anything with new, nor de we have pointers therefore we don't need to implement a Dt'r.
	void buildPuzzleFromFile(const string& fileName);
    PuzzlePiece* getPieceAt(int i);
    int getSize();
	int getMaxHeight();
	int getMaxWidth();
    int getTotalSum(){return _totalSum;};
	bool checkForMissingPieces(vector<int>& ids, vector<int> &idsFromFile);
	virtual void getPossibleSizes(vector<pair<int,int>> & result);
    bool isPrime(int n);
    virtual void checkCorners();
    void checkStraightEdges();
	void selAllPiecesValid();
	int totalStraightEdges(){ return _straightEdges[0] + _straightEdges[1]+_straightEdges[2]+_straightEdges[3];};
	int numStraightEdges(Edge edge){return _straightEdges[edge];};
private:
	void parseFirstLine(string line);
	int addPiece(PuzzlePiece& piece);


};


#endif /* PUZZLE_H_ */
