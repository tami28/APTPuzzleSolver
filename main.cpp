/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "Solver.h"
#include "PuzzleMatrix.h"
int main(){
    string infile = "testFile.txt";
#if DEBUG
    printf("IN DEBUG MODE!!!!\n");
#endif

    int row = 2;
    int col=2;
    bool fits;


    Puzzle puzzle = Puzzle("..\\tests\\TEST12.txt");
    Solver solver = Solver(puzzle);
    solver.solve();



//    puzzle = Puzzle("solvable2by2.txt");
//    row,col = 2,2;
//    solver = Solver(puzzle);
//    PuzzleMatrix pmx = PuzzleMatrix(row,col);
//    vector<int> indices = {1,2,3,4};
//    PuzzleMatrix* result = new PuzzleMatrix(row,col);
//    bool isSolved = solver._solveForSize(pmx ,  indices, result);
//
//    puzzle = Puzzle("NOTsolvable2by2.txt");
//    row,col = 2,2;
//    solver = Solver(puzzle);
//    pmx = PuzzleMatrix(row,col);
//    indices = {1,2,3,4};
//    result = new PuzzleMatrix(row,col);
//    isSolved = solver._solveForSize(pmx ,  indices, result);
//
//    puzzle = Puzzle("solvable3by2.txt");
//    row = 3;
//    col = 2;
//    solver = Solver(puzzle);
//    pmx = PuzzleMatrix(row,col);
//    indices = {1,2,3,4,5,6};
//    result = new PuzzleMatrix(row,col);
//    isSolved = solver._solveForSize(pmx ,  indices, result);



    printf("Done\n");
}

//TODO: close ErrorList


