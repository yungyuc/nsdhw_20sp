#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <math.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

float calc_angle(std::array<float, 2> v1, std::array<float, 2> v2)
{
    float len1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
    float len2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    float len = len1 * len2;

    if (len < pow(10, -4))
        throw std::invalid_argument("Division by zero not allowed!");

    float dot = v1[0] * v2[0] + v1[1] * v2[1];

    if (dot / len > 1 || dot / len < -1)
        throw std::invalid_argument("Invalid argument for acos calculation");

    return acos(dot / len);
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 angle"; // optional module docstring
    m.def("calc_angle", &calc_angle, "A function which calculates the angle between two 2D-vectors in radians");
}

