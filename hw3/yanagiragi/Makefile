all: main

CC=g++
OUTPUTFILE=a.out
CHECKFILE=result.txt

MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
MKLLINKLINE := \
	-L/${MKLROOT}/lib/intel64 \
	-lmkl_intel_lp64 -lmkl_core -lmkl_sequential \
	-lpthread -lm -ldl -m64

FLAGS=--std=c++11 -I${MKLROOT}/include ${MKLLINKLINE}

main: matrix.o main.o
	$(CC) -o $(OUTPUTFILE) matrix.o main.o $(FLAGS)

matrix.o: matrix.hpp matrix.cpp
	$(CC) $(FLAGS) -c matrix.cpp

main.o: main.cpp matrix.hpp
	$(CC) $(FLAGS) -c main.cpp

.PHONY:clean
clean:
	rm -f $(OUTPUTFILE) matrixls.o main.o $(CHECKFILE)

run:
	./$(OUTPUTFILE)

check:
	./$(OUTPUTFILE) > $(CHECKFILE)