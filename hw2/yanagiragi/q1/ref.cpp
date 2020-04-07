#include "ref.hpp"


float & Line::x(size_t it) const { check_range(it); return m_coord[it*2  ]; }
float & Line::x(size_t it)       { check_range(it); return m_coord[it*2  ]; }
float & Line::y(size_t it) const { check_range(it); return m_coord[it*2+1]; }
float & Line::y(size_t it)       { check_range(it); return m_coord[it*2+1]; }

/* Define the copy constructor */
Line::Line(Line const & other)
{
    if (other.m_coord)
    {
        if (m_coord) { delete[] m_coord; } // free unused buffer.
        m_coord = new float[other.m_size*2];
        m_size = other.m_size;
        std::copy_n(other.m_coord, other.m_size*2, m_coord);
    }
    else // the other object is empty.
    {
        if (m_coord)
        {
            delete[] m_coord; // free unused buffer.
            m_coord = nullptr;
            m_size = 0;
        }
    }
}

/* Define the move constructor */
Line::Line(Line && other)
{
    std::swap(other.m_size, m_size);
    std::swap(other.m_coord, m_coord);
}

/* Define the copy assignment operator */
Line & Line::operator=(Line const & other)
{
    if (this == &other) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    if (other.m_coord)
    {
        if (m_coord) { delete[] m_coord; }
        m_coord = new float[other.m_size*2];
        m_size = other.m_size;
        std::copy_n(other.m_coord, other.m_size*2, m_coord);
    }
    else
    {
        if (m_coord)
        {
            delete[] m_coord;
            m_coord = nullptr;
            m_size = 0;
        }
    }
    return *this;
}

/* Define the move assignment operator */
Line & Line::operator=(Line && other)
{
    if (this == &other) { return *this; } // don't move to self.
    std::swap(other.m_size, m_size);
    std::swap(other.m_coord, m_coord);
    return *this;
}

float Line::length() const
{
    float ret = 0;
    for (size_t it=1; it<size(); ++it) {
        ret += std::hypot(x(it) - x(it-1), y(it) - y(it-1));
    }
    return ret;
}
