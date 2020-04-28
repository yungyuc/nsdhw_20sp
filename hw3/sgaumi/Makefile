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
	-lpthread -lm -ldl

PYBINDLINE := -fPIC `python3 -m pybind11 --includes` \
	-std=c++11 -shared -O3

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-Wall \
	$(PYBINDLINE) \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}



.PHONY: default
default: _matrix

matrix: _matrix.cpp
	${CXX} ${CXXFLAGS} _matrix.cpp -o _matrix`python3-config --extension-suffix`

.PHONY: test
test:
	${CXX} ${CXXFLAGS} _matrix.cpp -o _matrix`python3-config --extension-suffix`
	pytest

.PHONY: clean
clean:
	rm -frd *.so __pycache__ .pytest_cache performance.txt _matrix
	touch performance.txt
