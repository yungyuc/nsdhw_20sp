CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PYTEST = pytest
PY = python3
FILENAME = _vector

all: $(FILENAME).*.so

$(FILENAME).*.so: $(FILENAME).cpp
	$(CXX) $(CXXFLAGS) `$(PY) -m pybind11 --includes` $(FILENAME).cpp -o $(FILENAME)`$(PY)-config --extension-suffix`

.PHONY: test
test: $(FILENAME).*.so
	$(PYTEST) pybind_test.py

.PHONY: clean
clean:
	rm -rf *.so __pycache__ .pytest_cache result.txt

.PHONY: check
check: $(FILENAME).*.so
	$(PYTEST) pybind_test.py > result.txt 
