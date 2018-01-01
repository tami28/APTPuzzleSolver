/*
 * Exceptions.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */


#include <sstream>
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
	_extraString1 = str;
}

Error::Error(ErrorType err, string str){
	_err = err;
	_extraInt1 = -1;
	_extraString1 = str;
}

ErrorType Error::getErrorType() const{
	return _err;
}

int Error::getIntInfo(){
	return _extraInt1;
}
string Error::getStrInfo(){
	return _extraString1;
}


std::string Error::toString(){
	string res = "";
	std::stringstream strm;
	switch (_err){
        case WRONG_FIRST_LINE_FORMAT:
            strm << "Invalid input file.";
            return strm.str();
		case WRONG_PIECE_FORMAT:
			strm << "Puzzle ID " << this->_extraInt1 << " has wrong data: " << this->_extraString1;
			return strm.str();
		case WRONG_PIECE_IDS:
			strm << "Puzzle of size " << Environment::getNumPieces()<< " cannot have the following IDs: " << this->_extraString1;
			return strm.str();
		case MISSING_PIECES:
			strm << "Missing puzzle element(s) with the following IDs: "<< this->_extraString1;
			return strm.str();
		case SUM_EDGES_NOT_ZERO:
			strm << "Cannot solve puzzle: sum of edges is not zero";
			return strm.str();
        case MISSING_CORNER:
            strm << "Cannot solve puzzle: missing corner element: " << this->_extraString1;
            return strm.str();
        case CORNERS_CANT_BE_COVERED:
            strm << "Cannot solve puzzle: Corners can't be covered";
            return strm.str();
		case COULD_NOT_FIND_SOLUTION:
			strm << "Cannot solve puzzle: it seems that there is no proper solution";
			return strm.str();
        case WRONG_NUM_STRAIGHT_EDGES:
            strm << "Cannot solve puzzle: wrong number of straight edges";
            return strm.str();

        default:
            break;
	}
	return "";
}


//---------------ErrorList:---------------------//

ErrorList* ErrorList::_errors;
bool ErrorList::_initialized;
int ErrorList::_numErrors;

//constructors & destructors:
ErrorList::ErrorList(){
	_errVec = std::vector<Error>();
	_initialized = true;
}



//Public methods:
ErrorList* ErrorList::getErrorList(){
	if (! ErrorList::_initialized){
		ErrorList::_errors = new ErrorList();
	}
	return ErrorList::_errors;
}

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
	_numErrors++;
}

int ErrorList::getNumErrors(){
	return _numErrors;
}

/*
 * Print all existing errors to file, in order of precedence.
 */
void ErrorList::toFile(){
	ofstream fout(Environment::getOutFilePath());
	if (fout.fail()) {
		cout << "Puzzle has error(s). Could not write errors to file\n";
	}
    sort(_errVec.begin(), _errVec.end(), [] (const Error &e1,const Error &e2) {
        return (int) e1.getErrorType() < (int) e2.getErrorType();});
	for (Error err : _errVec  ){
		fout << err.toString() << endl;

	}
	fout.close();
}
