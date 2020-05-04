FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-Wall -Wextra -Werror -shared \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

CXX = g++
PYBINDFLAGS := \
	-fPIC `python3 -m pybind11 --includes`

.PHONY: _matrix 
_matrix: _matrix.cpp
	${CXX} $^ ${CXXFLAGS} $(PYBINDFLAGS) -o $@`python3-config --extension-suffix`

.PHONY: clean
clean: 
	rm -rf _matrix *.so *.o performance.txt __pycache__/

.PHONY: test
test: _matrix
	py.test -v -l
