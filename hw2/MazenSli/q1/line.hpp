#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <iterator>  // for back_inserter
#include <algorithm> // for copy() and assign()

using namespace std;

class Line
{
public:
    // Basic constructors.
    Line() = default;              // default constructor.
    Line(Line const &);            // copy constructor.
    Line(Line &&);                 // move constructor.
    Line &operator=(Line const &); // copy assignment operator.
    Line &operator=(Line &&);      // move assignment operator.

    // Custom constructor.
    Line(size_t size) : m_coord(2 * size) {}

    // Desctructor.
    ~Line() = default;

    // Accessors.
    size_t size() const { return m_coord.size() / 2; }
    float const &x(size_t it) const;
    float &x(size_t it);
    float const &y(size_t it) const;
    float &y(size_t it);

private:
    // Private helper.
    void check_range(size_t it) const;
    // Member data.
    std::vector<float> m_coord;
}; /* end class Line */

