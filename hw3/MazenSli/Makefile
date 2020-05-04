CXX = g++
PYTHON_BIN=python3

MKLROOT = ${HOME}/opt/conda
PYTHONROOT = ${HOME}/opt/conda/include/python3.7m

MKLLINKLINE := ${MKLROOT}/lib/libmkl_intel_lp64.so ${MKLROOT}/lib/libmkl_sequential.so ${MKLROOT}/lib/libmkl_core.so -lpthread -lm -ldl

CXXFLAGS := -O3 -Wall -fPIC -shared -std=c++11 -I${MKLROOT}/include -I$(PYTHONROOT) ${MKLLINKLINE}

PYBINDFLAGS := -fPIC `python3 -m pybind11 --includes`

.PHONY: default
default: _matrix

.PHONY: _matrix
_matrix: _matrix.cpp
	${CXX} $< ${CXXFLAGS} $(PYBINDFLAGS) -o $@`python3-config --extension-suffix`

.PHONY: clean
clean:
	rm -frd *.so __pycache__ _matrix

.PHONY: test
test:
	$(CXX) $(CXXFLAGS) _matrix.cpp -o _matrix`python3-config --extension-suffix`
	pytest matrix_test.py
