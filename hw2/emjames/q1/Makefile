SHELL := /bin/bash
CXX = g++

main: main.o
	$(CXX) main.o -o main
main.o: main.cpp
	$(CXX) -c main.cpp -o main.o

.PHONY: clean
clean:
	rm -rf main *.o result.txt

run: 	main
	./main

.PHONY: check
check: main
	./main > result.txt
