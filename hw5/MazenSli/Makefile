CXX = g++

MKLROOT = ${HOME}/opt/conda

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.so \
	${MKLROOT}/lib/libmkl_sequential.so \
	${MKLROOT}/lib/libmkl_core.so \
	${MKLROOT}/lib/libmkl_avx2.so \
	-lpthread -lm -ldl

NUMPY := $(shell python3 -c "import numpy; print(numpy.get_include())") -I ./

CXXFLAGS := -O3 -Wall -fPIC -std=c++17 -I${MKLROOT}/include ${MKLLINKLINE} -I${NUMPY}

PYBINDFLAGS := -fPIC `python3 -m pybind11 --includes` -shared

.PHONY: default
default: _matrix

.PHONY: _matrix
_matrix: matrix.hpp
	${CXX} ../mod.cpp ${CXXFLAGS} ${PYBINDFLAGS} -o $@`python3-config --extension-suffix`

.PHONY: clean
clean:
	rm -frd *.so __pycache__ _matrix

