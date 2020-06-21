CXX := g++
CXXFLAGS ?= -std=c++17 -O3 -Wall
SRC = matrix.cpp
TARGET = _matrix.cpython-37m-x86_64-linux-gnu.so
TEST = test_matrix.py
BENCH = performance.py

MKLROOT ?= ${HOME}/opt/conda
INCLUDES += -m64 -I${MKLROOT}/include
LDFLAGS += -L${MKLROOT}/lib -Wl,--no-as-needed \
	-lmkl_rt -lpthread -lm -ldl

INCLUDES += `python -m pybind11 --includes`
CXXFLAGS += -shared -fPIC

.PHONY: all test performance format clean
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $(TARGET) $(LDFLAGS)

format: $(SRC) $(TEST) $(BENCH)
	clang-format -i $(SRC)
	yapf -i $(TEST) $(BENCH)

test: $(TARGET) $(TEST)
	pytest -sv $(TEST)

performance: $(TARGET) $(BENCH)
	python $(BENCH) > performance.txt

clean:
	rm -rf $(TARGET) performance.txt

