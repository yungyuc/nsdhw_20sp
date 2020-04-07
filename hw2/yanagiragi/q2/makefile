PYTHON_BIN=python3
all:
	c++ -O3 -Wall -shared -std=c++11 -fPIC `$(PYTHON_BIN) -m pybind11 --includes` main.cpp -o _vector`python3-config --extension-suffix`
clean:
	rm -f example`python3-config --extension-suffix`
test:
	$(PYTHON_BIN) -m pytest ./test_sample.py