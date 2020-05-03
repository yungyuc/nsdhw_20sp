#include <vector>
#include <cmath>
#include <cstdlib>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <exception>

namespace py = pybind11;


float calcul_angle(std::vector<float> const& U, std::vector<float> const& V){
	if(U.size()!=2 || V.size()!=2){
		throw std::runtime_error("vector length exception : the length of each vector must be 2");
	}
	else if(((U[0]*U[0]+U[1]*U[1])*(V[0]*V[0]+V[1]*V[1]))<0.000001){
		throw std::runtime_error("zero division exception : division by zero");
	}
	else{
		float sol = (U[0]*V[0]+U[1]*V[1]) / (sqrt((U[0]*U[0]+U[1]*U[1])*(V[0]*V[0]+V[1]*V[1])));
			std::cout<<sol<<std::endl;
			if(sol>0.999999){
				return 0.0;		
			}
			else{
				return acos(sol);
			}
	}
}


PYBIND11_MODULE(_vector, m) {
    m.doc() = "pybind11 angle between 2 vectors"; // optional module docstring

    m.def("calcul_angle", &calcul_angle, "A function which calculates the angle (rad) between 2 2D vectors in the Cartesian coordinate system");
}
