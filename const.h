#pragma once

#include <string>
#include <cstring>
#include<cstdlib>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <math.h>


enum Constraints { FEMALE = -1, STRAIGHT = 0, MALE = 1, NONE = 2};
enum outerFrameConstraints {LEFT_STRAIGHT= 1, TOP_STRAIGHT = 2, RIGHT_STAIGHT = 3, BOTTOM_STRAIGHT = 4,  FINAL = 5};
enum Edge {LEFT = 0, TOP = 1, RIGHT = 2, BOTTOM = 3, LAST = 4};
enum Corners {TL = 0, TR = 1, BL = 2, BR = 3, LAST_C};
enum Rotate {NO = 0, DEG90 = 90, DEG180 = 180, DEG270 = 270};

class Environment{
private:
    static unsigned int numPieces;
    static std::string outFilePath;
public:
    static unsigned int getNumPieces(){return numPieces;};
    static void setNumPieces(unsigned int num){numPieces = num;};
    static std::string getOutFilePath(){return outFilePath;};
    static void setOutFilePath(std::string path){ outFilePath = path;};
};
#define DEFAULT_OUTPUT_FILE "solution.out"
