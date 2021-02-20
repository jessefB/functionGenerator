#	Makefile for 'func'

# func.exe
main.exe : function.o path.o main.cpp
	g++ -o main.exe function.o path.o main.cpp

# Libraries
path.o : ./lib/path.h ./lib/path.cpp
	g++ -c ./lib/path.cpp

function.o : ./lib/function.h ./lib/function.cpp
	g++ -c ./lib/function.cpp