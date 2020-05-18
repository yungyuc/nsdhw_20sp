all: main

CC=g++
OUTPUTFILE=a.out
CHECKFILE=performance.txt

# MKL root for conda
MKL_ROOT=$(HOME)/opt/conda
MKL_LIB_DIR=$(MKL_ROOT)/lib
MKL_LIBS=$(MKL_LIB_DIR)/libmkl_def.so:$(MKL_LIB_DIR)/libmkl_avx2.so:$(MKL_LIB_DIR)/libmkl_core.so:$(MKL_LIB_DIR)/libmkl_intel_lp64.so:$(MKL_LIB_DIR)/libmkl_sequential.so

# MKL Root for my WSL
# MKL_ROOT ?= /opt/intel/mkl
# MKLLINKLINE := \
	-L/${MKL_ROOT}/lib/intel64 \
	-lmkl_intel_lp64 -lmkl_core -lmkl_sequential \
	-lpthread -lm -ldl -m64

MKLEXT ?= so
MKLLINKLINE := \
	${MKL_ROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKL_ROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKL_ROOT}/lib/libmkl_core.${MKLEXT} \
	${MKL_ROOT}/lib/libmkl_avx2.${MKLEXT}

MAJOR=ROW_MAJOR
PYTHON_BIN=python3
FLAGS=--std=c++11 -I${MKL_ROOT}/include ${MKLLINKLINE} -D $(MAJOR) -lpthread -lm -ldl

# use this line on my development env
# export LD_PRELOAD=/opt/intel/mkl/lib/intel64/libmkl_def.so:/opt/intel/mkl/lib/intel64/libmkl_avx2.so:/opt/intel/mkl/lib/intel64/libmkl_core.so:/opt/intel/mkl/lib/intel64/libmkl_intel_lp64.so:/opt/intel/mkl/lib/intel64/libmkl_intel_thread.so:/opt/intel/lib/intel64_lin/libiomp5.so

main: matrix.o main.o
	$(CC) -o $(OUTPUTFILE) matrix.o main.o $(FLAGS)

matrix.o: matrix.hpp matrix.cpp
	$(CC) -c matrix.cpp $(FLAGS)

main.o: main.cpp matrix.hpp
	$(CC) $(FLAGS) -c main.cpp

pybind:
	c++ -O3 -Wall -shared -std=c++11 -fPIC `$(PYTHON_BIN) -m pybind11 --includes` matrix_bind.cpp -o _matrix`python3-config --extension-suffix` $(FLAGS)

test: pybind
	LD_PRELOAD=$(MKL_LIBS) $(PYTHON_BIN) -m pytest ./test.py

.PHONY:clean
clean:
	rm -f $(OUTPUTFILE) matrix.o main.o $(CHECKFILE) _matrix`python3-config --extension-suffix`

run:
	./$(OUTPUTFILE)
