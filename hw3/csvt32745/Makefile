FLAGS ?=

UNAME_S := $(shell uname -s)

# ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= so
CXXFLAGS :=
# endif

# ifeq ($(UNAME_S),Linux)
# MKLROOT ?= ${HOME}/opt/conda
# MKLEXT ?= so
# CXXFLAGS := -Wl,--no-as-needed
# endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

PYBINDLINE := \
	-O3 -Wall -shared -std=c++11 \
	-fPIC `python3 -m pybind11 --includes` \
	# $^ -o $@`python3-config --extension-suffix`

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-I${MKLROOT}/include \
	${PYBINDLINE} \
	${MKLLINKLINE}
	# -std=c++17 -O3 -g -m64 \
	# ${FLAGS} \

.PHONY: default
default: _matrix

_matrix: _matrix.cpp
	${CXX} ${CXXFLAGS} $^ -o $@`python3-config --extension-suffix`

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
