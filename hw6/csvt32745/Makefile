FLAGS ?=

UNAME_S := $(shell uname -s)

MKLROOT ?= /opt/intel/mkl
MKLEXT ?= so
CXXFLAGS :=
NUMPY_PATH := $(shell python -c "import numpy;print(numpy.get_include())")

ifeq ($(UNAME_S),Linux)
MKLROOT := ${HOME}/opt/conda
MKLEXT := so
CXXFLAGS := -Wl,--no-as-needed
endif


MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx2.${MKLEXT}
	

PYBINDLINE := \
	-O3 -Wall -shared -std=c++11 \
	-fPIC `python3 -m pybind11 --includes` \
	# $^ -o $@`python3-config --extension-suffix`

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-I${MKLROOT}/include \
	-I ./ \
	-I${NUMPY_PATH} \
	${PYBINDLINE} \
	${MKLLINKLINE} \
	-lpthread -lm -ldl

SRC_NAME=matrix

.PHONY: default
default: _matrix
_matrix: matrix.hpp
	${CXX} ./bind.cpp ${CXXFLAGS} -o $@`python3-config --extension-suffix` 
	# ${CXX} $^ ${CXXFLAGS} -o $@`python3-config --extension-suffix` 

.PHONY: clean
clean:
	rm -rf *.so
