//
// Created by Tami on 05/12/2017.
//

#include "RotatePuzzle.h"


void RotatePuzzle::getPossibleSizes(vector<pair<int,int>> & result){
    int puzzleSize = _size;
    int sqr = (int) sqrt(puzzleSize) + 1;
    int totalStraightEdges = _straightEdges[0] + _straightEdges[1] + _straightEdges[2]+_straightEdges[3];
    for (int i = 1; i <= sqr; i++) {
        if (puzzleSize % i == 0 && 2*(i + puzzleSize/i) <= totalStraightEdges) { //Just make sure There's enough straight edges to cover it.
            result.push_back(pair<int, int>(i, puzzleSize / i));
        }
    }
}


void RotatePuzzle::checkCorners(){
    string errStr = "";

    int uniqueCorners = 0;
    //for (int corner = TL ; corner <= BR ; corner++){
    for (auto &pair : _corners){
        if (pair.second.empty()){
            break;
        }
        //for (auto piece =  _corners[(Corners) corner].begin(); piece != _corners[(Corners)corner].end(); ++piece){
        for(auto index : pair.second){
            for (auto &pair2 : _corners){
                if (pair.first != pair2.first){

                    pair2.second.erase(index);
                }

            }
        uniqueCorners ++;
        }
    }
    if (uniqueCorners < 4){
        errStr.append("<TL>");
    }
    if (uniqueCorners < 3){
        errStr.append("<TR>");
    }
    if (uniqueCorners < 2){
        errStr.append("<BL>");
    }
    if (uniqueCorners < 1){
        errStr.append("<BR>");
    }
    if (errStr != "") {
        (*ErrorList::getErrorList()).add(Error(MISSING_CORNER, errStr));
    }
}

