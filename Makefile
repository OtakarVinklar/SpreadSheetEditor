PROGRAM=SpreadSheetEditor
CC=g++
CFLAGS= -Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11 -g
LIBS= -lncurses -lm
# legend: $^ - all dependencies, #< - first dependenci, $@ - output

#output: dependency1 dependency2 dependency3 ...
#	What to do - how to compile (terminal command)

# all source files are stored in: src/
# all object files are genereted in: objs/filename.o

all: compile doc

run: compile
	./$(PROGRAM)

compile: $(PROGRAM)

doc: src/main.cpp src/Controller/CProgramController.hpp \
 src/Controller/CUIController.hpp src/Controller/../Constants.hpp \
 src/Sheet/CSheet.hpp \
 src/Sheet/CCellMatrix.hpp \
 src/Sheet/CCell.hpp \
 src/Sheet/CStringCell.hpp \
 src/Sheet/CNumberCell.hpp \
 src/Sheet/CEquationCell.hpp \
 src/Parser/CToken.hpp \
 src/Parser/CTokenExpEvaluator.hpp \
 src/Sheet/ECell.hpp \
 src/Parser/CTextParser.hpp
	doxygen Doxyfile


$(PROGRAM): objs/main.o objs/CProgramController.o objs/CSheet.o objs/CUIController.o \
 objs/CCell.o objs/CEquationCell.o objs/CNumberCell.o objs/CStringCell.o objs/CCellMatrix.o \
 objs/CToken.o objs/CTextParser.o objs/CTokenExpEvaluator.o objs/CCellMatrix.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

objs/main.o: src/main.cpp src/Controller/CProgramController.hpp \
 src/Controller/CUIController.hpp src/Controller/../Constants.hpp \
 src/Sheet/CSheet.hpp \
 src/Sheet/CCellMatrix.hpp \
 src/Sheet/CCell.hpp \
 src/Sheet/CStringCell.hpp \
 src/Sheet/CNumberCell.hpp \
 src/Sheet/CEquationCell.hpp \
 src/Parser/CToken.hpp \
 src/Parser/CTokenExpEvaluator.hpp \
 src/Sheet/ECell.hpp \
 src/Parser/CTextParser.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CProgramController.o: src/Controller/CProgramController.cpp \
 src/Controller/CProgramController.hpp src/Controller/CUIController.hpp \
 src/Constants.hpp src/Sheet/CSheet.hpp \
 src/Sheet/CCellMatrix.hpp \
 src/Sheet/CCell.hpp \
 src/Sheet/CStringCell.hpp \
 src/Sheet/CNumberCell.hpp \
 src/Sheet/CEquationCell.hpp \
 src/Parser/CToken.hpp \
 src/Parser/CTokenExpEvaluator.hpp \
 src/Sheet/ECell.hpp \
 src/Parser/CTextParser.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CSheet.o: src/Sheet/CSheet.cpp src/Sheet/CSheet.hpp \
 src/Sheet/CCellMatrix.hpp src/Sheet/CCell.hpp src/Constants.hpp \
 src/Sheet/CStringCell.hpp src/Sheet/CNumberCell.hpp \
 src/Sheet/CEquationCell.hpp src/Parser/CToken.hpp \
 src/Parser/CTokenExpEvaluator.hpp src/Sheet/ECell.hpp \
 src/Parser/CTextParser.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CUIController.o: src/Controller/CUIController.cpp \
 src/Controller/CUIController.hpp src/Constants.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CCellMatrix.o: src/Sheet/CCellMatrix.cpp src/Sheet/CCellMatrix.hpp \
 src/Sheet/CCell.hpp src/Constants.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CCell.o: src/Sheet/CCell.cpp src/Sheet/CCell.hpp \
 src/Constants.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CEquationCell.o: src/Sheet/CEquationCell.cpp src/Sheet/CEquationCell.hpp \
 src/Sheet/CCell.hpp src/Constants.hpp \
 src/Parser/CToken.hpp \
 src/Parser/CTokenExpEvaluator.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CNumberCell.o: src/Sheet/CNumberCell.cpp src/Sheet/CNumberCell.hpp \
 src/Sheet/CCell.hpp src/Constants.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CStringCell.o: src/Sheet/CStringCell.cpp src/Sheet/CStringCell.hpp \
 src/Sheet/CCell.hpp src/Constants.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CToken.o: src/Parser/CToken.cpp src/Parser/CToken.hpp \
 src/Constants.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CTextParser.o: src/Parser/CTextParser.cpp src/Parser/CTextParser.hpp \
 src/Sheet/ECell.hpp src/Parser/CToken.hpp \
 src/Constants.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/CTokenExpEvaluator.o: src/Parser/CTokenExpEvaluator.cpp \
 src/Parser/CTokenExpEvaluator.hpp src/Parser/CToken.hpp \
 src/Constants.hpp | objs
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf sheet objs ./$(PROGRAM) doc

objs:
	mkdir objs

depend:
	g++ -MM ./src/*.cpp ./src/Controller/*.cpp ./src/Parser/*.cpp ./src/Sheet/*.cpp
