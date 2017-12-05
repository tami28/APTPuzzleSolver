//
// Created by Yoav on 08-Nov-17.
//

#ifndef PROJECT_PUZZLEPIECE_H
#define PROJECT_PUZZLEPIECE_H

#include <string>
#include <cstring>
#include<cstdlib>
#include "const.h"
#include "Exceptions.h"

#define ALTERNATIVE_ZERO_STRING "-2"
#define ALTERNATIVE_ZERO_INT -2
#define DEFAULT_PIECE_ID -1
#define DEFAULT_EDGE Constraints::NONE


using namespace std;

class PuzzlePiece  {
protected:
    Constraints edges[4];
    int id;
    int encoding;
public:
    PuzzlePiece(); // Empty c'tor
    PuzzlePiece(int id, int edges[4]);//PuzzlePiece(const PuzzlePiece& otherPiece); //copy c'tor
    PuzzlePiece(const std::string& inputFileLine, vector<int> &idsFromFile); //c'tor from file-line
    int getId();
    int getEncoding();
    Constraints getConstraint(Edge edge, Rotate rotation);
    Constraints getConstraint(Edge edge);
    Constraints getOppositeConstraint(Edge edge);
    bool canConnect(PuzzlePiece& other, Edge edge); //can connect with other on given edge
    string getConstraintStr(Rotate rotation);
    bool getIsValid();
    void setIsValid(bool value);
    bool isValid; //TODO should be private..
};

/*
 * No need here for Dt'r because we use simple types. & assigment of type arr[int] is not allocated by us, so we are cool.
 * Note: didn't delete operator=, because we use it, and against my initial thoughts it works even on the array of constraints.
 * Same things go for the copy Ct'r which we need because of the use of vectors.
 */


#endif //PROJECT_PUZZLEPIECE_H
