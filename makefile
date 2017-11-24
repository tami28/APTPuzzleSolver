COMP = g++-5.3.0
#put all your object files here
OBJS = Exceptions.o PuzzlePiece.o Puzzle.o PuzzleMatrix.o Solver.o main.o #objects should be placed here!
#The executabel filename DON'T CHANGE
EXEC = ex1
CPP_COMP_FLAG = -std=c++11 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
main.o: main.cpp Puzzle.h PuzzlePiece.h const.h Exceptions.h Solver.h PuzzleMatrix.h#put dependencies here!
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
	rm -f $(OBJS) $(EXEC)
	
