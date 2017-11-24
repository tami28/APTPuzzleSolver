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
enum Constraints { FEMALE = -1, STRAIGHT = 0, MALE = 1, NONE = 9};
enum Edge {LEFT = 0, TOP = 1, RIGHT = 2, BOTTOM = 3, LAST};

enum Corners {TL = 0, TR, BL, BR, LAST_C};

extern std::string outFilePath;

#define DEFAULT_OUTPUT_FILE "solution.out"