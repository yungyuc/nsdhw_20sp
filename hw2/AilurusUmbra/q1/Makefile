CXX = g++
CXXFLAGS = -std=c++11 -Wall

EXEC = line-test
OBJ = main.o line.o

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@	

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: run
run: $(EXEC)
	./$(EXEC)

.PHONY: check
check: $(EXEC)
	./$(EXEC) > result.txt

.PHONY: clean
clean:
	rm -f *.o $(EXEC) result.txt
