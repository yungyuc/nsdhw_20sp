all: main

CC=g++
FLAGS=--std=c++11
OUTPUTFILE=a.out

main: line.o main.o
	$(CC) $(FLAGS) -o $(OUTPUTFILE) line.o main.o

line.o: line.hpp line.cpp
	$(CC) $(FLAGS) -c line.cpp

main.o: main.cpp line.hpp
	$(CC) $(FLAGS) -c main.cpp

#%.o:%.cpp
#	$(CC) $(FLAGS) -c $^ $@

.PHONY:clean
clean:
	rm -f $(OUTPUTFILE) line.o main.o