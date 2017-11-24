/*
 * Exceptions.h
 *
 *  Created on: 10 ����� 2017
 *      Author: Tami
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include "const.h"
#include <fstream>
using namespace std;

enum ErrorType{ //Values represent order of output to file.
    WRONG_FIRST_LINE_FORMAT = 0,
    MISSING_PIECES = 1,
    WRONG_PIECE_IDS = 2,
    WRONG_PIECE_FORMAT = 3,
    WRONG_NUM_STRAIGHT_EDGES = 4,
    MISSING_CORNER = 5,
    CORNERS_CANT_BE_COVERED = 6,
    SUM_EDGES_NOT_ZERO = 7,
    COULD_NOT_FIND_SOLUTION = 8,
    _WRONG_PIECE_ID,
    MISSING_SIZE,
    SUCCESS

};




class Error{
private:
	ErrorType _err;
	int _extraInt1;
	std::string _extraString1;
public:
	Error(ErrorType err);
	Error(ErrorType err, int id);
	Error(ErrorType err, std::string str);
	Error(ErrorType err, std::string str, int id);
	ErrorType getErrorType() const;
    int getIntInfo();
    string getStrInfo();
	std::string toString();
};


class ErrorList{
private:
	static bool _initialized;
	static int _numErrors;
	static ErrorList* _errors;
	std::vector<Error> _errVec;
	ErrorList();
	//virtual ~ErrorList();

public:
	static ErrorList* getErrorList();
	void add(Error e);
	static void close();
	void toFile();
	static int getNumErrors();
};



#endif /* EXCEPTIONS_H_ */
