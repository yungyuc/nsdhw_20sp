CXX := g++
CXXFLAGS ?= -std=c++17 -O3 -Wall
SRC = matrix.hpp
TARGET = _matrix
TEST = test_matrix.py
BENCH = performance.py
NUMPY_PATH := $(shell python -c "import numpy;print(numpy.get_include())")

MKLROOT ?= ${HOME}/opt/conda
INCLUDES += -m64 -I${MKLROOT}/include
LDFLAGS += -L${MKLROOT}/lib -Wl,--no-as-needed \
	-lmkl_rt -lpthread -lm -ldl

INCLUDES += `python -m pybind11 --includes`
CXXFLAGS += -shared -fPIC

CXXFLAGS := ${CXXFLAGS} \
            -I${NUMPY_PATH}

.PHONY: all test performance format clean
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) ../mod.cpp $(CXXFLAGS) $(INCLUDES) -o $@`python3-config --extension-suffix` 
	#$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $(TARGET) $(LDFLAGS)

format: $(SRC) $(TEST) $(BENCH)
	clang-format -i $(SRC)
	yapf -i $(TEST) $(BENCH)

test: $(TARGET) $(TEST)
	pytest -sv $(TEST)

performance: $(TARGET) $(BENCH)
	python $(BENCH) > performance.txt

clean:
	rm -rf $(TARGET) performance.txt

