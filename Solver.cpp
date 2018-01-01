//
// Created by Yoav on 18-Nov-17.
//

#include "Solver.h"




//Solver::Solver(){} //empty c'tor



/*
 * returns a vector of all pairs of ints r,c s.t r*c == puzzle.get_Size()
 */
std::vector<pair<int, int>> Solver::getPossiblePuzzleSizes(){
    vector<pair<int,int>> result;
    _puzzle.get()->getPossibleSizes(result);
    return result;
}


int Solver::performValidityChecks(){
    if (0 == ErrorList::getNumErrors()){
        //check for wrong-num-of-straight-edges-error:
        _puzzle.get()->checkStraightEdges();
        //check for missing corner error:
        if ( !(_puzzle.get()->isPrime(numPieces)) || numPieces== 1 ||hasSingleRowColSolution() ) {
            _puzzle.get()->checkCorners();
        }
        //check for sum-not-zero error:
        if(0 != _puzzle.get()->getTotalSum()){
            (*ErrorList::getErrorList()).add(Error(SUM_EDGES_NOT_ZERO));
        }
    }
    if (ErrorList::getNumErrors() > 0) {
        return -1;
    }
    return 0;
}


void Solver::solve(){
    int row, col;
    bool solved = false;
    //Check:
    if (performValidityChecks() != 0){ return; }

    // Get all possible puzzle sizes:
    std::vector<pair<int, int>> sizesVec = getPossiblePuzzleSizes();
    int threadIndex = 0;
    std::vector<std::thread> threads;
    auto sizesPerThread = divideSizesToThreads(sizesVec);
    for (auto size : sizesPerThread){
        if (threadIndex == _numThreads-1) { //thread of index _numThreads-1 is our main thread (will be launched after launching all other threads)
            threadSolveForSize(size, threadIndex++);
        }else {
            threads.push_back(std::thread(&Solver::threadSolveForSize, this, size, threadIndex++));
        }
    }
    for (std::thread &th : threads){ th.join(); }

    if (this->_solved){ //One of the threads successfully _solved
        this->_solution.toFile(outFilePath);
    }
    else {

        (*ErrorList::getErrorList()).add(Error(COULD_NOT_FIND_SOLUTION));
    }
}



vector<vector<pair<int,int>>> Solver::divideSizesToThreads(vector<pair<int,int>> allPossibleSizes){
    vector<vector<pair<int,int>>> res;
    //sort possible sizes by "squareness":
    std::sort(allPossibleSizes.begin(), allPossibleSizes.end(),
         [](const pair<int,int> &p1, const pair<int,int> &p2) -> bool
         {
             return std::abs(p1.first-p1.second) < std::abs(p2.first-p2.second);
         });
    //more square sizes go to smaller groups
    int numGroups = _numThreads <= allPossibleSizes.size() ? _numThreads : allPossibleSizes.size();
    int i=0;
    for (i=0; i<numGroups; i++){res.push_back(vector<pair<int,int>>{});}
    i=0;
    int directionFlag = 1;
    for (auto& size : allPossibleSizes){
        res[i].push_back(size);
        if (res.size() == 1) { continue; } //for single thread.
        i+= directionFlag;
        switch (directionFlag) {
            case 1:
                if (i == numGroups-1) { directionFlag = 0; }
                break;
            case 0:
                directionFlag = i == 0 ? 1 : -1;
                break;
            case -1:
                if (i == 0) { directionFlag = 0; }
                break;
        }

    }
    return res;
}

void Solver::threadSolveForSize(vector<pair<int,int>> sizes, int threadIndex){
    bool success = false;
    for (auto size : sizes){
        int row = size.first;
        int col = size.second;
        PuzzleMatrix pm = PuzzleMatrix(row, col);
        vector<int> usedIDs;
        setStep(row, col, threadIndex);
        success =_solveForSize(pm, usedIDs, threadIndex);
        if ( success ) { // Find a solution for size (row,col)
            std::lock_guard<std::mutex> lock(_declaringSolvedMutex);
            this->_solved = true;
            this->_solution = PuzzleMatrix(pm);
        } //mutex released here.
    }
}

bool Solver::_solveForSize(PuzzleMatrix& pm, vector<int> usedIDs, int threadIndex) {
    if (this->_solved) { return false; }

    if (!Puzzle::withRotations && numPieces > MIN_NUM_PIECES_TO_CHECK_SUFFICIENT_CONSTRAINTS
        && usedIDs.size() > numPieces*(PIECES_RATIO_TO_CHECK_SUFFICIENT_CONSTRAINTS)
        &&  !(SolvabilityVerifier(_puzzle , pm, usedIDs)).verifySolvabilityConstraints()) {
            return false;
    }
    int constraints[4] = {NONE, NONE, NONE, NONE};
    int row = steppersMap[threadIndex].get()->i;
    int col = steppersMap[threadIndex].get()->j;
    pm.constraintsOfCell(row, col, constraints, steppersMap[threadIndex].get()->getType());
    unordered_set<int> badPieces;
    set<IDandRotation> relevantPieceIDs = _puzzle.get()->constraintsTable.getIDsFittingConstraints(constraints);
    int i;
    Rotate rotation;
    for (auto PieceIDandRotation : relevantPieceIDs){
        i = PieceIDandRotation.first;
        rotation = PieceIDandRotation.second;
        if (_isFitForCell(i, badPieces, usedIDs, rotation)){
            std::map<Constraints , int> requiredCountersSnapshot = pm.getRequiredCounters();
            std::map<outerFrameConstraints, int> requieredFrameConstraintsSnapshot = pm.getRequiredFrameConstraints();
            pm.assignPieceToCell(_puzzle.get()->getPieceAt(i),rotation, steppersMap[threadIndex].get()->i,steppersMap[threadIndex].get()->j);
            vector<int> newUsedIDs(usedIDs);
            newUsedIDs.push_back(i);
            if(solverFinished(newUsedIDs, threadIndex)){
                return true;
            }
            if (_solveForSize(pm, newUsedIDs, threadIndex)){
                return true;
            }
            pm.setRequiredCounters(requiredCountersSnapshot);
            pm.setRequiredFrameConstraints(requieredFrameConstraintsSnapshot);
            steppersMap[threadIndex].get()->prevStep();
            badPieces.insert((*_puzzle.get()->getPieceAt(i)).getConstraintsKey(rotation));
        }
    }
    return false;
}


bool Solver::_isFitForCell(int i, std::unordered_set<int>& badPieces, vector<int> usedIDs, Rotate rotation){
    return (find(usedIDs.begin(),usedIDs.end(),i) == usedIDs.end() &&
            badPieces.find((*_puzzle.get()->getPieceAt(i)).getConstraintsKey(rotation)) == badPieces.end());
}




bool Solver::piecefitsConstrains(PuzzlePiece& piece, char constraints[4]){
    for(int e = LEFT; e <= BOTTOM; ++e){
        if (!(0 == ((int)piece.getConstraint((Edge)e) +(int)constraints[e]) || constraints[e] == NONE)) {
            return false;
        }
    }
    return true;
}


bool Solver::hasSingleRowColSolution(){
    PuzzleMatrix row_pm = PuzzleMatrix(1, _puzzle.get()->getSize());
    vector<int> usedIDs;

    steppersMap[0] = std::make_unique<StepRow>(1, _puzzle.get()->getSize());
    if (_solveForSize(row_pm, usedIDs, 0)){
        return true;
    }
    usedIDs.clear();
    PuzzleMatrix col_pm = PuzzleMatrix(_puzzle.get()->getSize(), 1);
    steppersMap[0] = std::make_unique<StepRow>(_puzzle.get()->getSize(),1);
    if (_solveForSize(col_pm, usedIDs, 0)){
        return true;
    }
    return false;
}

bool Solver::solverFinished(vector<int> usedIDs, int threadIndex){
    if ((!steppersMap[threadIndex].get()->nextStep()) && usedIDs.size() == numPieces){
        return true;
    }
    return false;
}


void Solver::setStep(int nrow, int ncol, int threadIndex){
    //Decide which is the best stepper, meaning by what order to go:
    int colDiff = _puzzle.get()->numStraightEdges(LEFT) - nrow;
    int rowDiff = _puzzle.get()->numStraightEdges(TOP) - ncol;

    if (nrow <= 2){
        steppersMap[threadIndex] = std::make_unique<StepRow>(nrow,ncol);
        return;
    }
    if(ncol <= 2){
        steppersMap[threadIndex] = std::make_unique<StepCol>(nrow,ncol);
        return;
    }
    if (_puzzle.get()->totalStraightEdges() <STRAIGHT_EDGES_RATIO_FOR_FRAME_STEPS*numPieces*4){
        steppersMap[threadIndex] = std::make_unique<StepFrame>(nrow, ncol);
    }

    if (Puzzle::withRotations){
        steppersMap[threadIndex] = std::make_unique<StepRow>(nrow,ncol);
        return;
    }
    //This only  matters for non rotate puzzles, go by the one with less options - first col or first row
    if (colDiff >= rowDiff){
        steppersMap[threadIndex] = std::make_unique<StepRow>(nrow,ncol);
    } else{
        steppersMap[threadIndex] = std::make_unique<StepCol>(nrow,ncol);
    }

}

