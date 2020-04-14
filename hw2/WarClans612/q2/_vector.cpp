// Developer: Wilbert (wilbert.phen@gmail.com)

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <stdexcept>
#include <cmath>

namespace py = pybind11;

float angle(std::array<float, 2> vec1, std::array<float, 2> vec2)
{
    float s_length1 = vec1[0] * vec1[0] + vec1[1] * vec1[1];
    float s_length2 = vec2[0] * vec2[0] + vec2[1] * vec2[1];
    if (s_length1 == 0 || s_length2 == 0) 
        throw std::invalid_argument("Vector length cannot be 0(zero)");

    float dot_product = vec1[0] * vec2[0] + vec1[1] * vec2[1];
    float a = dot_product / sqrt(s_length1 * s_length2);

    if (a >= 1.0)
        return 0.0;
    else if (a <= -1.0)
        return M_PI;
    else
        return acos(a);
}

PYBIND11_MODULE(_vector, m) {
    m.def("angle", &angle, "Calculates angle in radians between 2 vectors with 2-dim Cartesian coordinate");
} 
