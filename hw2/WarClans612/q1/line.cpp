// Developer: Wilbert (wilbert.phen@gmail.com)

#include "line.hpp"

#include <cstdint>
#include <algorithm>
#include <utility>

/* Define the copy constructor */
Line::Line(Line const & other)
    : m_coord(other.m_coord.size()*2, 0)
{
    std::copy(other.m_coord.begin(), other.m_coord.end(), m_coord.begin());
}

/* Define the move constructor */
Line::Line(Line && other)
    : m_coord(other.m_coord.size()*2, 0)
{
    
    other.m_coord.swap(m_coord);
}

/* Define the copy assignment operator */
Line & Line::operator=(Line const & other)
{
    if (this == &other) { return *this; } // don't move to self.
    // This part is the same as what's in the copy constructor.
    m_coord.resize(other.m_coord.size()*2, 0);
    std::copy(other.m_coord.begin(), other.m_coord.end(), m_coord.begin());
    return *this;
}

/* Define the move assignment operator */
Line & Line::operator=(Line && other)
{
    if (this == &other) { return *this; } // don't move to self.
    m_coord.resize(other.m_coord.size()*2, 0);
    other.m_coord.swap(m_coord);
    return *this;
}
