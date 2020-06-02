FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
#MKLROOT ?= /home/asministrateur/anaconda3
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx2.${MKLEXT} \
	${MKLROOT}/lib/libmkl_avx512.${MKLEXT} \
	${MKLROOT}/lib/libmkl_def.${MKLEXT} \
	-lpthread -lm -ldl

PYBINDLINE := -fPIC `python3 -m pybind11 --includes` \
	-std=c++17 -shared -O3

BY_MOD := $(shell python3 -c "import numpy; print(numpy.get_include())") \
		-I ./

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-Wall \
	$(PYBINDLINE) \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS} \
	-I${BY_MOD}

MODNAME := ../mod.cpp

.PHONY: default
default: matrix

matrix: matrix.hpp
	${CXX} ${CXXFLAGS} ${MODNAME} -o _matrix`python3-config --extension-suffix`

.PHONY: test
test:
	${CXX} ${CXXFLAGS} ${MODNAME} -o _matrix`python3-config --extension-suffix`
	pytest
	cat performance.txt

.PHONY: clean
clean:
	rm -frd *.so __pycache__ ../__pycache__ .pytest_cache ../.pytest_cache  performance.txt _matrix
	touch performance.txt
