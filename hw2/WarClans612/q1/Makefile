CXX = g++
cflag = -Wall -Wextra -Werror

.PHONY: line
line: linemain.o line.o
	$(CXX) $^ -o $@ $(cflag)

.PHONY: run
run: line
	./line

.PHONY: check
check: line
	./line >result.txt 2>&1

.PHONY: clean
clean: 
	rm -f line *.o result.txt

%.o: %.cpp line.hpp
	$(CXX) -c $< -o $@ $(cflag)