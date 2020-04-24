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

PYTHON_BIN=python3

# source /opt/intel/mkl/bin/mklvars.sh intel64
# export LD_LIBRARY_PATH=/usr/local/lib:/opt/intel/mkl/lib/intel64:$LD_LIBRARY_PATH
# export LD_PRELOAD=/opt/intel/mkl/lib/intel64/libmkl_def.so:/opt/intel/mkl/lib/intel64/libmkl_avx2.so:/opt/intel/mkl/lib/intel64/libmkl_core.so:/opt/intel/mkl/lib/intel64/libmkl_intel_lp64.so:/opt/intel/mkl/lib/intel64/libmkl_intel_thread.so:/opt/intel/lib/intel64_lin/libiomp5.so

main: matrix.o main.o
	$(CC) -o $(OUTPUTFILE) matrix.o main.o $(FLAGS)

matrix.o: matrix.hpp matrix.cpp
	$(CC) $(FLAGS) -c matrix.cpp

main.o: main.cpp matrix.hpp
	$(CC) $(FLAGS) -c main.cpp

pybind:
	c++ -O3 -Wall -shared -std=c++11 -fPIC `$(PYTHON_BIN) -m pybind11 --includes` matrix_bind.cpp -o _vector`python3-config --extension-suffix` $(FLAGS)

.PHONY:clean
clean:
	rm -f $(OUTPUTFILE) matrixls.o main.o $(CHECKFILE)

run:
	./$(OUTPUTFILE)

check:
	./$(OUTPUTFILE) > $(CHECKFILE)