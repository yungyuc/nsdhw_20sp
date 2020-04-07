CXX = g++

default: angle

angle: angle.cpp
	$(CXX) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` angle.cpp -o angle`python3-config --extension-suffix`

clean:
	rm -f angle *.so

test:
	python3 pythontest.py
