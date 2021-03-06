CXX := g++
#CXXFLAGS ?= -std=c++17 -O3 -Wall
SRC = matrix.hpp
TARGET = _matrix
TEST = test_matrix.py
BENCH = performance.py
NUMPY_PATH := $(shell python -c "import numpy;print(numpy.get_include())")


MKLROOT ?= ${HOME}/opt/conda
#MKLROOT ?= /opt/intel/mkl
CXXFLAGS := -Wl,--no-as-needed
#############
MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.so \
	${MKLROOT}/lib/libmkl_sequential.so \
	${MKLROOT}/lib/libmkl_core.so \
	${MKLROOT}/lib/libmkl_avx2.so \
	#-lpthread -lm -ldl
#CXXFLAGS := -O3 -Wall -fPIC -std=c++17 -I${MKLROOT}/include ${MKLLINKLINE} -I${NUMPY_PATH}

PYBINDFLAGS := -O3 -Wall -shared -std=c++17 -fPIC `python3 -m pybind11 --includes` #-shared

##############

#INCLUDES += -m64 -I${MKLROOT}/include
#LDFLAGS += -L${MKLROOT}/lib -Wl,--no-as-needed \
	#-lmkl_rt -lpthread -lm -ldl

#INCLUDES += `python -m pybind11 --includes`
#CXXFLAGS += -shared -fPIC

#CXXFLAGS := ${CXXFLAGS} \
#            -I${NUMPY_PATH}

CXXFLAGS := ${CXXFLAGS} \
	-I${MKLROOT}/include \
	-I${NUMPY_PATH} \
	-I ./ \
	${PYBINDFLAGS} \
	${MKLLINKLINE} \
	-lpthread -lm -ldl

.PHONY: all test performance format clean
all: $(TARGET)

$(TARGET): $(SRC)
	#$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $(TARGET) $(LDFLAGS)
	#$(CXX) mod.cpp $(CXXFLAGS) $(INCLUDES) -o $@`python3-config --extension-suffix` 
	#${CXX} mod.cpp ${CXXFLAGS} ${PYBINDFLAGS} -o $@`python3-config --extension-suffix`
	${CXX} ../mod.cpp ${CXXFLAGS} -o $@`python3-config --extension-suffix` 
	

format: $(SRC) $(TEST) $(BENCH)
	clang-format -i $(SRC)
	yapf -i $(TEST) $(BENCH)

test: $(TARGET) $(TEST)
	pytest -sv $(TEST)

performance: $(TARGET) $(BENCH)
	python $(BENCH) > performance.txt

clean:
	rm -rf $(TARGET) performance.txt

