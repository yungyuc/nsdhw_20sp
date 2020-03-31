// Developer: Wilbert (wilbert.phen@gmail.com)

#pragma once

#include <iostream>
#include <vector>

class Line
{
public:
    // Basic constructors.
    Line() = default; // default constructor.
    Line(Line const & ); // copy constructor.
    Line(Line       &&); // move constructor.
    Line & operator=(Line const & ); // copy assignment operator.
    Line & operator=(Line       &&); // move assignment operator.
    // Custom constructor.
    Line(size_t size) : m_coord(size*2, 0) {}
    // Desctructor.
    ~Line() = default; // default destructor
    // Accessors.
    size_t size() const { return m_coord.size()/2; }
    float   x(size_t it) const { return m_coord.at(it*2  ); }
    float & x(size_t it)       { return m_coord.at(it*2  ); }
    float   y(size_t it) const { return m_coord.at(it*2+1); }
    float & y(size_t it)       { return m_coord.at(it*2+1); }
private:
    // Member data.
    std::vector<float> m_coord; // memory buffer for the points.
}; /* end class Line */
