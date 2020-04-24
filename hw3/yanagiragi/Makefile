all: main

CC=g++
OUTPUTFILE=a.out
CHECKFILE=performance.txt

# MKL root for conda
MKL_ROOT=$(HOME)/opt/conda
MKL_LIB_DIR=$(MKL_ROOT)/lib
MKL_LIBS=$(MKL_LIB_DIR)/libmkl_def.so:$(MKL_LIB_DIR)/libmkl_avx2.so:$(MKL_LIB_DIR)/libmkl_core.so:$(MKL_LIB_DIR)/libmkl_intel_lp64.so:$(MKL_LIB_DIR)/libmkl_sequential.so

# MKL Root for my WSL
# MKLROOT ?= /opt/intel/mkl
# MKLLINKLINE := \
	-L/${MKLROOT}/lib/intel64 \
	-lmkl_intel_lp64 -lmkl_core -lmkl_sequential \
	-lpthread -lm -ldl -m64

MKLLINKLINE := \
	-L/${MKL_LIB_DIR} \
	-lpthread -lm -ldl -m64

MAJOR=ROW_MAJOR
PYTHON_BIN=python3
FLAGS=--std=c++11 -I${MKLROOT}/include ${MKLLINKLINE} -D $(MAJOR)

# use this line on my development env
# export LD_PRELOAD=/opt/intel/mkl/lib/intel64/libmkl_def.so:/opt/intel/mkl/lib/intel64/libmkl_avx2.so:/opt/intel/mkl/lib/intel64/libmkl_core.so:/opt/intel/mkl/lib/intel64/libmkl_intel_lp64.so:/opt/intel/mkl/lib/intel64/libmkl_intel_thread.so:/opt/intel/lib/intel64_lin/libiomp5.so

main: matrix.o main.o
	$(CC) -o $(OUTPUTFILE) matrix.o main.o $(FLAGS)

matrix.o: matrix.hpp matrix.cpp
	$(CC) $(FLAGS) -c matrix.cpp

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

check:
	./$(OUTPUTFILE) > $(CHECKFILE)
