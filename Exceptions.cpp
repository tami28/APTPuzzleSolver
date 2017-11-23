/*
 * Exceptions.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */


#include <sstream>
#include "Exceptions.h"

map<ErrorType ,string> ErrorTypeToStringMap = {
		{WRONG_FIRST_LINE_FORMAT, "Error: Wrong first line format.\n"}
};


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
	_extraString1 = str;
}

Error::Error(ErrorType err, string str){
	_err = err;
	_extraInt1 = -1;
	_extraString1 = str;
}

ErrorType Error::getErrorType(){
	return _err;
}

std::string Error::toString(){
	string res = "";
	std::stringstream strm;
	switch (_err){
		case WRONG_PIECE_FORMAT:
			strm << "Puzzle ID " << this->_extraInt1 << " has wrong data: " << this->_extraString1;
			return strm.str();


	}
}


//---------------ErrorList:---------------------//

ErrorList* ErrorList::_errors;
bool ErrorList::_initialized;

//constructors & destructors:
ErrorList::ErrorList(){
	_errVec = std::vector<Error>();
	_initialized = true;
}

//TODO: is this needed?
/*
ErrorList::~ErrorList(){
	_errVec.clear();
}
*/

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



/*
 * Print all existing errors to file, in order of precedence.
 */
void ErrorList::toFile(){
	ofstream fout(outFilePath);
	for (Error err : _errVec){ //TODO: make sure we are going over this in order of precendece.
		fout << err.toString() << endl;
	}
	fout.close();

}