#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>

float calculate(std::vector<float> &first, std::vector<float> &second) {
    const float first_x = first[0];
    const float first_y = first[1];
    const float second_x = second[0];
    const float second_y = second[1];
    const float dot = first_x * second_x + first_y * second_y;
    const float first_magnitude = sqrtf(first_x * first_x + first_y * first_y);
    const float second_magnitude = sqrtf(second_x * second_x + second_y * second_y);
    return std::acos( dot / (first_magnitude * second_magnitude) );
}

PYBIND11_MODULE(example, m) {
    m.doc() = "nsdhw_20sp hw2 (yanagiragi)"; // optional module docstring
    m.def("calc", &calculate, "Calculates the angle (in radians) between two vectors in the 2-dimensional Cartesian coordinate system");
}