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

CXXGCOMPILE := -std=c++17 -O3 \
	-Wall -Wextra -Werror

CXX = g++
PYBINDFLAGS := \
	-fPIC `python3 -m pybind11 --includes`

.PHONY: _matlink
_matlink: _matrix _main.o _matrix.o _tiler.o
	$(CXX) -shared -o _matrix`python3-config --extension-suffix` _main.o _matrix.o _tiler.o

_matrix: _main.cpp
	${CXX} $< ${CXXFLAGS} $(PYBINDFLAGS) -o $@`python3-config --extension-suffix`

_main.o: _main.cpp
	$(CXX) -c $< ${CXXGCOMPILE} $(PYBINDFLAGS) -o $@ -fPIC

_matrix.o: _matrix.cpp _matrix.hpp Makefile
	$(CXX) -c $< ${CXXGCOMPILE} -I${MKLROOT}/include -o $@ -fPIC

%.o: %.cpp %.hpp Makefile
	$(CXX) -c $< ${CXXGCOMPILE} -o $@ -fPIC

.PHONY: clean
clean: 
	rm -rf _matrix *.so *.o performance.txt __pycache__/ .pytest_cache/ .cache/

.PHONY: test
test: clean _matlink Makefile
	#$(CXX) -o _matrix _matrix.o _tiler.o _block.o
	#./_matrix
	#python test_matrix.py
	py.test -v -l
