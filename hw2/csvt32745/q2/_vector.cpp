#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "float.h"

namespace py = pybind11;

float radian_between_2dvecs(std::array<float, 2> vecA, std::array<float, 2> vecB)
{
    float a = vecA[0]*vecA[0] + vecA[1]*vecA[1];
    float b = vecB[0]*vecB[0] + vecB[1]*vecB[1];
    if(a < FLT_MIN || b < FLT_MIN)
        return 0.0;

    float d = (vecA[0]*vecB[0] + vecA[1]*vecB[1])/sqrt(a*b);

    return acos(d);
}

PYBIND11_MODULE(_vector, m) {
    m.def("radian_between_2dvecs", &radian_between_2dvecs, "Calculate radians between 2 vectors with 2-dim");
}