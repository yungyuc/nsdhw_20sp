CXX = g++
cflag = -Wall -Wextra -Werror

.PHONY: _vector
_vector: _vector.cpp
	$(CXX) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` $< -o $@`python3-config --extension-suffix`

.PHONY: clean
clean: 
	rm -f _vector *.so

.PHONY: test
test:
	python3 pytest.py