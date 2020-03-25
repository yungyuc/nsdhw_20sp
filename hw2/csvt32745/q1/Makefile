CXX = g++

.PHONY: default
default: main
main: line.cpp
	$(CXX) $^ -o $@.o

.PHONY: clean
clean:
	rm -rf *.o result.txt

.PHONY: run
run:
	./main.o

.PHONY: check
check:
	./main.o > result.txt