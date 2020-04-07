#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

float calculate(std::vector<float> &first, std::vector<float> &second) {
    const float first_x = first[0];
    const float first_y = first[1];
    const float second_x = second[0];
    const float second_y = second[1];
    const float x = first_x - second_x;
    const float y = first_y - second_y;
    const float magnitude = sqrtf(x * x + y * y);
    return std::atan2(y / magnitude, x / magnitude);
}

PYBIND11_MODULE(example, m) {
    m.doc() = "nsdhw_20sp hw2 (yanagiragi)"; // optional module docstring
    m.def("calc", &calculate, "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system");
}