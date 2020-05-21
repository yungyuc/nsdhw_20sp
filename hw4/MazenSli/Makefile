CXX = g++

MKLROOT = ${HOME}/opt/conda

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.so \
	${MKLROOT}/lib/libmkl_sequential.so \
	${MKLROOT}/lib/libmkl_core.so \
	${MKLROOT}/lib/libmkl_avx2.so \
	-lpthread -lm -ldl

CXXFLAGS := -O3 -Wall -fPIC -std=c++11 -I${MKLROOT}/include ${MKLLINKLINE}

PYBINDFLAGS := -fPIC `python3 -m pybind11 --includes` -shared

.PHONY: default
default: _matrix

.PHONY: _matrix
_matrix: _matrix.cpp
	${CXX} $< ${CXXFLAGS} ${PYBINDFLAGS} -o $@`python3-config --extension-suffix`

.PHONY: clean
clean:
	rm -frd *.so __pycache__ _matrix performance.txt

.PHONY: test
test: _matrix
	pytest -v -l
