//
// Created by Tami on 17/12/2017.
//

#include "Steper.h"

bool StepRow::nextStep(){
    if ( i == nrow-1){
        if (j == ncol-1){
            return false;
        }
        j++;
        return true;
    }
    if(j==ncol -1){
        i++;
        j = 0;
    } else{
        j++;
    }
    return true;
}

bool StepRow::prevStep(){
    if ( i == 0){
        if (j == 0){
            return false;
        }
        j--;
        return true;
    }
    if(j==0){
        i--;
        j = ncol-1;
    } else{
        j--;
    }
    return true;
}

bool StepFrame::nextStep(){
    //on first row till corner + 1:
    if ( i == 0){
        if (j < ncol-1){
            j++;
        } else{
            i++; //turn the corner
        }
        return true;
    }
    //last col till corner:
    if(j==ncol -1){
        if(i < nrow - 1){
            i++;
        } else{ //turn the corner:
            j--;
        }
        return true;
    }
    //last row:
    if (i == nrow-1){
        if (j>0){
            j--;
        }else{
            i--; //turn the corner
        }
        return true;
    }
    //first col:
    if (j==0){
        if(i>1){ //not to go over 0,0 again!
            i--;
        } else{ //finished frame, start inside!
            i=1;
            j=1;
        }
        return true;
    }
    //go over inside by rows:
    if ( i == nrow-2){
        if (j == ncol-2){
            return false;
        }
        j++;
        return true;
    }
    if(j==ncol -2){
        i++;
        j = 1;
    } else{
        j++;
    }
    return true;
}

bool StepFrame::prevStep(){
    //on first row till corner:
    if ( i == 0){
        if (j > 0){
            j--;
        } else{ //corner:
            return false;
        }
        return true;
    }
    //last col till corner:
    if(j==ncol -1){
        if(i >0){
            i--;
        } else{ //return on first row..
            j--;
        }
        return true;
    }
    //last row:
    if (i == nrow-1){
        if (j<ncol -1){
            j++;
        }else{ //turn the corner to last col
            i--;
        }
        return true;
    }
    //first col:
    if (j==0) {
        if (i < nrow - 1) { //not to go over 0,0 again!
            i++;
        } else {
            j++;
        }
        return true;
    }
    //    //go over inside by rows:
    if (i == 1 && j==1) {
        j--;
        return true;
    }
    if (j == 1){
        j = ncol -2;
        i--;
        return true;
    }else{
        j--;
    }
    return true;
}

bool StepCol::nextStep(){
    if ( j == ncol-1){
        if (i == nrow-1){
            return false;
        }
        i++;
        return true;
    }
    if(i==nrow-1){
        j++;
        i = 0;
    } else{
        i++;
    }
    return true;
}

bool StepCol::prevStep(){
    if ( j == 0){
        if (i == 0){
            return false;
        }
        i--;
        return true;
    }
    if(i==0){
        j--;
        i = nrow-1;
    } else{
        i--;
    }
    return true;
}

StepType StepRow::getType(){
    return ROW;
}
StepType StepFrame::getType() {
    return FRAME;
}

StepType StepCol::getType() {
    return COL;
}