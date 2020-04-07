#include "line.hpp"

/* Define the copy constructor */
Line::Line(Line const &other)
{
    m_coord.clear();
    copy(other.m_coord.begin(), other.m_coord.end(), back_inserter(m_coord));
}

/* Define the move constructor */
Line::Line(Line &&other)
{
    std::swap(other.m_coord, m_coord);
}

/* Define the copy assignment operator */
Line &Line::operator=(Line const &other)
{
    if (this == &other)
    {
        return *this;
    } // don't move to self.
    copy(other.m_coord.begin(), other.m_coord.end(), back_inserter(m_coord));
    return *this;
}

/* Define the move assignment operator */
Line &Line::operator=(Line &&other)
{
    if (this == &other)
    {
        return *this;
    } // don't move to self.
    std::swap(other.m_coord, m_coord);
    return *this;
}

void Line::check_range(size_t it) const
{
    if (it >= m_coord.size())
    {
        throw std::out_of_range("Line index out of range");
    }
}

float &Line::x(size_t it)
{
    check_range(it);
    return m_coord[it * 2];
}

float const &Line::x(size_t it) const
{
    check_range(it);
    return m_coord[it * 2];
}

float &Line::y(size_t it)
{
    check_range(it);
    return m_coord[it * 2 + 1];
}

float const &Line::y(size_t it) const
{
    check_range(it);
    return m_coord[it * 2 + 1];
}
