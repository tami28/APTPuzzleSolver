/*
 * Exceptions.h
 *
 *  Created on: 10 бреб„ 2017
 *      Author: Tami
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <vector>
#include <string>
using namespace std;

enum ErrorType{SUCCESS,
		MISSING_PIECE,
		WRONG_PIECE_ID,
		WRONG_PIECE_FORMAT,
		MISSING_CORNER,
		SUM_EDGES_NOT_ZERO,
		NO_SOLOUTION,
		MISSING_SIZE,
		WRONG_FORMET
};

class Error{
private:
	ErrorType _err;
	int _extraInt1;
	std::string _extraString1;
public:
	Error(ErrorType err);
	Error(ErrorType err, int id);
	Error(ErrorType err, string str);
	Error(ErrorType err, string str, int id);
	ErrorType getErrorType();
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
};



#endif /* EXCEPTIONS_H_ */
