#include <pybind11/pybind11.h>
#include <vector>
#include <cmath>

// These 2 constants could be found in float.h
#define FLT_MIN 1.175494351e-38F
#define FLT_EPSILON 1.192092896e-07F

namespace py = pybind11;

class Point{
  public:

    Point(float px, float py) { coord = {px, py}; }
    std::array<float, 2> coord;
    const float& operator[] (int index) const { return coord[index]; };
};


float get_angle(const Point& p1, const Point& p2){

    float len1 = p1[0] * p1[0] + p1[1] * p1[1];
    float len2 = p2[0] * p2[0] + p2[1] * p2[1];
    
    // avoid 0 division  
    if (len1 < FLT_MIN + FLT_EPSILON || 
        len2 < FLT_MIN + FLT_EPSILON)
      return 0.0;

    float dot = p1[0] * p2[0] + p1[1] * p2[1];

    float a = dot / sqrt(len1 * len2);

    if(a >= 1.0)
        return 0.0;
    else if(a <= -1.0)
        return M_PI;
    else
        return acos(a);
}

PYBIND11_MODULE(_vector, m) {
  
  py::class_<Point>(m, "_Point")
        .def(py::init<float, float>());

  m.def("get_angle",
        &get_angle, 
        "A function which calculate angle between two points." );
}
