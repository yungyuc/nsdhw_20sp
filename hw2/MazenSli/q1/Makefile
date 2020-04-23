CXX = g++

line: line.o main.o
	$(CXX) $^ -o $@

run: line
	./line

line.o: line.cpp line.hpp
	$(CXX) -c $< -o $@

main.o: main.cpp line.hpp
	$(CXX) -c $< -o $@

check:
	./line > result.txt

clean:
	rm -rf *.o result.tct
