#pragma once

#include <string>
#include <cstring>
#include<cstdlib>
#include <iostream>
#include <utility>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <set>
#include <math.h>

#define INVALID_PIECE_ID -9

extern int numPieces;
enum Constraints { FEMALE = -1, STRAIGHT = 0, MALE = 1, NONE = 2};
enum outerFrameConstraints {LEFT_STRAIGHT= 1, TOP_STRAIGHT = 2, RIGHT_STAIGHT = 3, BOTTOM_STRAIGHT = 4,  FINAL = 5};
enum Edge {LEFT = 0, TOP = 1, RIGHT = 2, BOTTOM = 3, LAST = 4};

enum Corners {TL = 0, TR = 1, BL = 2, BR = 3, LAST_C};

enum Rotate {NO = 0, DEG90 = 90, DEG180 = 180, DEG270 = 270};

extern bool withRotations;
#define DEFAULT_OUTPUT_FILE "solution.out"
extern std::string outFilePath;

//TODO: need not to use global variables