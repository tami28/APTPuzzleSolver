/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "PuzzleMatrix.h"
int main(){
	string line = "2 -1 0 1 -1";
    //PuzzlePiece p  = PuzzlePiece(line);
    string infile = "testFile.txt";
    Puzzle puzzle = Puzzle(infile);
    int row = 3;
    int col=2;
    PuzzleMatrix pm = PuzzleMatrix(row,col);

    PuzzlePiece ppiece = (puzzle.getPieceAt(0));

    pm.isFit(&ppiece,0,0);


    /*for (int i=0; i<row; i++){
        for (int j=0; j<col; j++){
            for (int k=LEFT ; k<=BOTTOM; k++){
               printf("i=%d,j=%d,k=%d,  Constraint=%d \n", i,j,k, pm(i,j,(Edge)k));
            }}}
    */
    printf("Done\n");
}

//TODO: close ErrorList


