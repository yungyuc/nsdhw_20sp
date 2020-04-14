.PHONY: default
default: _vector

_vector: _vector.cpp
	c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` _vector.cpp -o _vector`python3-config --extension-suffix`

.PHONY: clean
clean:
	rm -frd *.so __pycache__ .pytest_cache

.PHONY: test
test:
	pytest
