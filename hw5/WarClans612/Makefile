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

NUMPYROOT := $(shell python -c "import numpy; print(numpy.get_include())")

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++14 -O3 -g -m64 \
	-Wall -Wextra  -shared \
	-I${MKLROOT}/include \
	-I${NUMPYROOT} \
	${MKLLINKLINE} \
	${FLAGS}

CXXGCOMPILE := -std=c++14 -O3 \
	-Wall -Wextra  \
	-I${NUMPYROOT}

CXX = g++
PYBINDFLAGS := \
	-fPIC `python3 -m pybind11 --includes`

.PHONY: _matlink
_matlink: matrix mod.o matrix.o _tiler.o
	$(CXX) -shared -o _matrix`python3-config --extension-suffix` mod.o matrix.o _tiler.o

matrix: mod.cpp
	${CXX} $< ${CXXFLAGS} $(PYBINDFLAGS) -o $@`python3-config --extension-suffix`

mod.o: mod.cpp
	$(CXX) -c $< ${CXXGCOMPILE} $(PYBINDFLAGS) -o $@ -fPIC

matrix.o: matrix.cpp matrix.hpp Makefile
	$(CXX) -c $< ${CXXGCOMPILE} -I${MKLROOT}/include -o $@ -fPIC

%.o: %.cpp %.hpp Makefile
	$(CXX) -c $< ${CXXGCOMPILE} -o $@ -fPIC

.PHONY: clean
clean: 
	rm -rf matrix *.so *.o performance.txt __pycache__/ .pytest_cache/ .cache/

.PHONY: test
test: clean _matlink Makefile
	#$(CXX) -o matrix matrix.o _tiler.o _block.o
	#./matrix
	#python testmatrix.py
	py.test -v -l