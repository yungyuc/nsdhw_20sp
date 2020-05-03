CXX = g++

.PHONY: default
default: line

line: line.cpp
	$(CXX) line.cpp -Wall -o line

.PHONY: run
run:
	./line

.PHONY: check
check:
	./line > result.txt

.PHONY: clean
clean:
	rm -f line result.txt