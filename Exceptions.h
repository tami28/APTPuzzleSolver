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

enum ErrorType{SUCCESS,
		MISSING_PIECE,
		_WRONG_PIECE_ID,
        WRONG_PIECE_IDS,
		PIECE_ID_NAN,
		WRONG_PIECE_FORMAT,
		MISSING_CORNER,
		SUM_EDGES_NOT_ZERO,
		COULD_NOT_FIND_SOLUTION,
		MISSING_SIZE,
		WRONG_FIRST_LINE_FORMAT
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
	ErrorType getErrorType();
    int getIntInfo();
    string getStrInfo();
	std::string toString();
};


class ErrorList{
private:
	static bool _initialized;
	static ErrorList* _errors;
	std::vector<Error> _errVec;
	ErrorList();
	//virtual ~ErrorList();

public:
	static ErrorList* getErrorList();
	void add(Error e);
	static void close();
	void toFile();
};



#endif /* EXCEPTIONS_H_ */
