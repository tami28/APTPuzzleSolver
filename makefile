COMP = g++-5.3.0
#put all your object files here
OBJS = Exceptions.o PuzzlePiece.o Puzzle.o PuzzleMatrix.o Solver.o main.o #objects should be placed here!
#The executabel filename DON'T CHANGE
EXEC = ex1
CPP_COMP_FLAG = -std=c++11 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
Exceptions.o: Exceptions.cpp
    $(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Puzzle.o: Puzzle.cpp
    $(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PuzzlePiece.o: PuzzlePiece.cpp
    $(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PuzzleMatrix.o: PuzzleMatrix.cpp
    $(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Solver.o: Solver.cpp
    $(COMP) $(CPP_COMP_FLAG) -c $*.cpp

main.o: main.cpp Puzzle.h PuzzlePiece.h const.h Exceptions.h Solver.h PuzzleMatrix.h#put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)

