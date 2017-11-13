/*
 * Exceptions.cpp
 *
 *  Created on: 12 бреб„ 2017
 *      Author: Tami
 */

#include "Exceptions.h"


//Constructors for ERror:
Error::Error(ErrorType err){
	_err = err;
	_extraInt1 = -1;
	//_extraString1 = nullptr;
}

Error::Error(ErrorType err, int id){
	_err = err;
	_extraInt1 = id;
	//_extraString1 = nullptr;
}

Error::Error(ErrorType err, string str, int id){
	_err = err;
	_extraInt1 = id;
	_extraString1 = new string(str);
}

Error::Error(ErrorType err, string str){
	_err = err;
	_extraInt1 = -1;
	_extraString1 = new string(str);
}

ErrorType Error::getErrorType(){
	return _err;
}


//---------------ErrorList:---------------------//

//constructors & destructors:
ErrorList::ErrorList(){
	_errVec = std::vector<Error>();
}

//TODO: is this needed?
ErrorList::~ErrorList(){
	_errVec.clear();
}

//Public methods:
ErrorList* ErrorList::getErrorList(){
	if (! ErrorList::_initialized){
		ErrorList::_errors = new ErrorList();
	}
	return ErrorList::_errors;
}

//TODO: find design where close doesn't have to be closed?
//delete the errorList
void ErrorList::close(){
	if (ErrorList::_errors != 0 && ErrorList::_initialized){
		delete ErrorList::_errors;
		ErrorList::_errors = 0;
	}
}


void ErrorList::add(Error e){
	//This runs on the instance of the class so it must be already initialized.
	_errVec.push_back(e);
}
