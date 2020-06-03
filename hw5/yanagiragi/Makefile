all: main pybind

CC=g++
OUTPUTFILE=a.out
CHECKFILE=performance.txt

# MKL root for conda
MKL_ROOT=$(HOME)/opt/conda
MKL_LIB_DIR=$(MKL_ROOT)/lib
MKL_LIBS=$(MKL_LIB_DIR)/libmkl_def.so:$(MKL_LIB_DIR)/libmkl_avx2.so:$(MKL_LIB_DIR)/libmkl_core.so:$(MKL_LIB_DIR)/libmkl_intel_lp64.so:$(MKL_LIB_DIR)/libmkl_sequential.so

MKLEXT ?= so
MKLLINKLINE := \
	${MKL_ROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKL_ROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKL_ROOT}/lib/libmkl_core.${MKLEXT} \
	${MKL_ROOT}/lib/libmkl_avx2.${MKLEXT}

MAJOR=ROW_MAJOR
PYTHON_BIN=python3
FLAGS=--std=c++14 -I${MKL_ROOT}/include ${MKLLINKLINE} -lpthread -lm -ldl
# use c++14 for std::conditional_t and etc.

main: pybind
	
pybind:
	c++ -O3 -Wall -shared -fPIC `$(PYTHON_BIN) -m pybind11 --includes` mod.cpp -o _matrix`python3-config --extension-suffix` $(FLAGS)

#test: pybind
#	LD_PRELOAD=$(MKL_LIBS) $(PYTHON_BIN) -m pytest ./test.py

.PHONY:clean
clean:
	rm -f $(OUTPUTFILE) $(CHECKFILE) _matrix`python3-config --extension-suffix`

run:
	./$(OUTPUTFILE)
