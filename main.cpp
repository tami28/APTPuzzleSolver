/*
 * main.cpp
 *
 *  Created on: 12 ����� 2017
 *      Author: Tami
 */

#include "Puzzle.h"
#include "PuzzleMatrix.h"
int main(){
    string infile = "testFile.txt";
    Puzzle puzzle = Puzzle(infile);
    int row = 3;
    int col=2;
    bool fits;
    PuzzleMatrix pm = PuzzleMatrix(row,col);

    PuzzlePiece ppiece = (puzzle.getPieceAt(0));
    fits = pm.isFit(&ppiece, 0,0);
    if (fits) {pm.assignPieceToCell(&ppiece, 0,0);}

    ppiece = (puzzle.getPieceAt(1));
    fits = pm.isFit(&ppiece, 0,1);
    if (fits) {pm.assignPieceToCell(&ppiece, 0,1);}

    ppiece = (puzzle.getPieceAt(2));
    fits=pm.isFit(&ppiece, 1,0);
    if (fits) {pm.assignPieceToCell(&ppiece, 1,0);}

    ppiece = (puzzle.getPieceAt(3));
    fits=pm.isFit(&ppiece, 1,1);
    if (fits) {pm.assignPieceToCell(&ppiece, 1,1);}


    ppiece = (puzzle.getPieceAt(4));
    fits = pm.isFit(&ppiece, 2,1);
    if (fits) {pm.assignPieceToCell(&ppiece, 2,1);}

    printf("Done\n");
}

//TODO: close ErrorList


