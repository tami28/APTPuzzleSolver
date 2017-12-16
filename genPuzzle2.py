from random import choice, shuffle

LEFT=1
TOP=2
RIGHT=3
BOTTOM=4

FEMALE=-1
MALE=1
STRAIGHT=0

CONSTRAINS = [FEMALE, MALE, STRAIGHT]
def genPuzzle(row,col, tofile=None, sort=False):
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
        #f.write("\n\nSOLUTION: "+str(solutionOrder))
        print "\n\nSOLUTION: "+str(solutionOrder)
        f.close()

    return puzzle, solutionOrder

def flatten(pzzl):
    res=[]
    for r in pzzl:
        for e in r:
           res.append(e)
    return res
    


def ppz(p):
    for row in p:
        for col in row:
            print str(col)[1:len(str(col))-1]



def validateSolution(puzzleFile, solFile, prnt = False, rotate = False):

    solf = open(solFile, 'r')
    solf = solf.read()
    sol = solf.split("\n")
    sol[len(sol) - 1] = sol[len(sol) - 1][:-1]
    sol = [l.split(" ") for l in sol]
    # sol = [[int(e) for e in l] for l in sol]
    sol = sol[:-1]

    puzf = open(puzzleFile,'r')
    puzf = puzf.read()
    puzzle = puzf.split("\n")
    puzzle = [e for e in puzzle[1:] if len(e.split(" ")) == 5]
    #puzzle = puzzle[1:-1]
    puzzle = {int(l.split(" ")[0]):[int(e) for e in l.split(" ")[1:]] for l in puzzle}
    if rotate:
        for row in sol:
            i=0
            for p in row:
                if '[' in p: # i.e prev was rotated
                    piece_id_to_rotate = int(row[i-1])
                    rotate_by = int(p[1:len(p)-1])
                    puzzle[piece_id_to_rotate] = rotate_piece(puzzle[piece_id_to_rotate], rotate_by)
                    row.remove(p)
                i+=1

    nrows = len(sol)
    ncols = len(sol[0])
    valid = True
    for row in range(nrows):
        for col in range(ncols):
                        pid = int(sol[row][col])
                        if row > 0:
                            tnid = int(sol[row-1][col])
                            if puzzle[tnid][3] != puzzle[pid][1]*(-1):
                                if prnt: print "fail 1"
                                valid=False
                        else:
                            if puzzle[pid][1] != 0:
                                if prnt: print "fail 2"
                                valid= False
                        if row < nrows-1:
                            bnid = int(sol[row+1][col])
                            if puzzle[bnid][1] != puzzle[pid][3]*(-1):
                                if prnt: print "fail 3"
                                valid=False
                        if row==nrows-1:
                            if puzzle[pid][3]!=0:
                                if prnt: print "fail 4"
                                valid=False
                        if col > 0:
                            lnid = int(sol[row][col-1])
                            if puzzle[lnid][2] != puzzle[pid][0]*(-1):
                                if prnt: print "fail 5"
                                valid=False
                        if col == 0:
                            if puzzle[pid][0] != 0:
                                if prnt: print "fail 6"
                                valid= False
                        if col < ncols-1:
                            rnid = int(sol[row][col+1])
                            if puzzle[rnid][0] != puzzle[pid][2]*(-1):
                                if prnt: print "fail 7"
                                valid=False
                        if col == ncols-1:
                            if puzzle[pid][2] != 0:
                                if prnt: print "fail 8"
                                valid= False
                    
    return valid                    


def rotate_piece(piece, rotate_by):
    if rotate_by >=90:
        rotate_by = rotate_by/90
    rotated_piece = [9,9,9,9]
    for i in [0,1,2,3]:
        offset =  ( (i-rotate_by)%4 )
        rotated_piece[i] = piece[offset]
    return rotated_piece


#genPuzzle(3,3,r'C:\Users\Yoav\CLionProjects\APTPuzzleSolver\Tests\gen3.txt')
    
#print validateSolution(r'C:\Users\Yoav\CLionProjects\APTPuzzleSolver\Tests\rotCorn.txt', r'C:\Users\Yoav\CLionProjects\APTPuzzleSolver\Tests\SOLUTION.txt', True, rotate=True)
