from random import choice, shuffle

LEFT=1
TOP=2
RIGHT=3
BOTTOM=4

FEMALE=-1
MALE=1
STRAIGHT=0

CONSTRAINS = [FEMALE, MALE, STRAIGHT]
def genPuzzle(row,col, tofile=None):
    puzzle = [[[-99,-9,-9,-9,-9] for j in range(col)] for i in range(row)]


    solutionOrder = []
    possible_indices = [n for n in range(1,1+row*col)]
    rrow = range(row)
    rcol = range(col)
    for i in rrow:
        for j in rcol:
            if i==0:
                puzzle[i][j][TOP] = STRAIGHT
            else:
                if puzzle[i-1][j][BOTTOM] != -9:
                    puzzle[i][j][TOP] = -1*puzzle[i-1][j][BOTTOM]
                else:
                    puzzle[i][j][TOP] = choice(CONSTRAINS)
            if i==row-1:
                puzzle[i][j][BOTTOM] = STRAIGHT
            else:
                if puzzle[i+1][j][TOP] != -9:
                    puzzle[i][j][BOTTOM] = -1*puzzle[i+1][j][TOP]
                else:
                    puzzle[i][j][BOTTOM] = choice(CONSTRAINS)
            if j==0:
                puzzle[i][j][LEFT] = STRAIGHT
            else:
                if puzzle[i][j-1][RIGHT] != -9:
                    puzzle[i][j][LEFT] = -1*puzzle[i][j-1][RIGHT]
                else:
                    puzzle[i][j][LEFT] = choice(CONSTRAINS)
            if j==col-1:
                puzzle[i][j][RIGHT] = STRAIGHT
            else:
                if puzzle[i][j+1][LEFT] != -9:
                    puzzle[i][i][RIGHT] = -1*puzzle[i][j+1][LEFT]
                else:
                    puzzle[i][j][RIGHT] = choice(CONSTRAINS)
            rind = choice(possible_indices)
            puzzle[i][j][0] = rind
            possible_indices.remove(rind)
            solutionOrder.append(rind)

    if tofile:
        f=open(tofile,"w")
        f.write("NumElements={}\n".format(row*col))
        copypuzzle =[r for r in puzzle]
        shuffle(copypuzzle)
        for row in copypuzzle:
            for col in row:
                f.write(str(col)[1:len(str(col))-1].replace(",","")+"\n")
        f.write("\n\nSOLUTION: "+str(solutionOrder))
        f.close()

    return puzzle, solutionOrder


def ppz(p):
    for row in p:
        for col in row:
            print (str(col)[1:len(str(col))-1])
