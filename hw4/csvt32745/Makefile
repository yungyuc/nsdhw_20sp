FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= so
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
	${MKLROOT}/lib/libmkl_avx2.${MKLEXT}
	

PYBINDLINE := \
	-O3 -Wall -shared -std=c++11 \
	-fPIC `python3 -m pybind11 --includes` \
	# $^ -o $@`python3-config --extension-suffix`

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-I${MKLROOT}/include \
	${PYBINDLINE} \
	${MKLLINKLINE} \
	-lpthread -lm -ldl

SRC_NAME=_matrix

.PHONY: default
default: _matrix

_matrix: _matrix.cpp
	${CXX} $^ ${CXXFLAGS} -o $@`python3-config --extension-suffix` 
	
.PHONY: test
test:
	${CXX} ${SRC_NAME}.cpp ${CXXFLAGS} -o ${SRC_NAME}`python3-config --extension-suffix` 
	python3 -m pytest -v -s

.PHONY: clean
clean:
	rm -rf *.so
