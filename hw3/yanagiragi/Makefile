all: main

CC=g++
FLAGS=--std=c++11
OUTPUTFILE=a.out
CHECKFILE=result.txt

main: matrix.o main.o
	$(CC) $(FLAGS) -o $(OUTPUTFILE) matrix.o main.o

matrix.o: matrix.hpp matrix.cpp
	$(CC) $(FLAGS) -c matrix.cpp

main.o: main.cpp matrix.hpp
	$(CC) $(FLAGS) -c main.cpp

.PHONY:clean
clean:
	rm -f $(OUTPUTFILE) matrix.o main.o $(CHECKFILE)

run:
	./$(OUTPUTFILE)

check:
	./$(OUTPUTFILE) > $(CHECKFILE)