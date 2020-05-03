all: main

CC=g++
FLAGS=--std=c++11
OUTPUTFILE=a.out
CHECKFILE=result.txt

main: line.o main.o
	$(CC) $(FLAGS) -o $(OUTPUTFILE) line.o main.o

line.o: line.hpp line.cpp
	$(CC) $(FLAGS) -c line.cpp

main.o: main.cpp line.hpp
	$(CC) $(FLAGS) -c main.cpp

.PHONY:clean
clean:
	rm -f $(OUTPUTFILE) line.o main.o $(CHECKFILE)

run:
	./$(OUTPUTFILE)

check:
	./$(OUTPUTFILE) > $(CHECKFILE)