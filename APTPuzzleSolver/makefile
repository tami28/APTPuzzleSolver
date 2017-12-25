COMP = g++-5.3.0
#put all your object files here
OBJS = Exceptions.o PuzzlePiece.o Puzzle.o PuzzleMatrix.o RotatePuzzle.o SolvabilityVerifier.o Steper.o ConstraintsTable.o Solver.o main.o #objects should be placed here!
#The executabel filename DON'T CHANGE
EXEC = ex2
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
Exceptions.o: Exceptions.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Puzzle.o: Puzzle.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
RotatePuzzle.o: RotatePuzzle.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PuzzlePiece.o: PuzzlePiece.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PuzzleMatrix.o: PuzzleMatrix.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Steper.o: Steper.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
SolvabilityVerifier.o: SolvabilityVerifier.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
ConstraintsTable.o: ConstraintsTable.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Solver.o: Solver.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

main.o: main.cpp const.h Exceptions.h Puzzle.h PuzzlePiece.h Solver.h PuzzleMatrix.h RotatePuzzle.h SolvabilityVerifier.h Steper.h ConstraintsTable.h #put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)

