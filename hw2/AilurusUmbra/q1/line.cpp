#include "line.hpp"

Line::Line(size_t size) : _length(size)
{
    m_coord = new std::vector<coord_2d>(size);
}

Line::~Line()
{
    delete m_coord;
}

size_t Line::size() const
{
    return _length;
}

// copy constructor 
Line::Line(Line const &other) : _length(other.size())
{
    if(other.m_coord)
    {
        m_coord = new std::vector<coord_2d>(other.size());
        std::copy(other.m_coord->begin(), other.m_coord->end(), m_coord->begin());
    }
    else
    {
        m_coord = nullptr;
    }
}

// move constructor
Line::Line(Line &&other) : _length(other._length), m_coord(other.m_coord)
{
    other.m_coord = nullptr;
}

// copy assignment
Line & Line::operator=(Line const &other)
{
    if(this == &other)
        return *this;

    delete m_coord;
    
    if(other.m_coord)
    {
        m_coord = new std::vector<coord_2d>(other.size());
        std::copy(other.m_coord->begin(), other.m_coord->end(), m_coord->begin());

        _length = other.size();
    }

    return *this;
}

// move assignment
Line & Line::operator=(Line &&other)
{
    if(this == &other)
        return *this;
    
    delete m_coord;
    
    m_coord = other.m_coord;
    other.m_coord = nullptr;
    
    _length = other.size();

    return *this;
}

float & Line::x(size_t it) const
{
    return m_coord->at(it).first;
}

float & Line::x(size_t it)
{
    return m_coord->at(it).first;
}

float & Line::y(size_t it) const
{
    return m_coord->at(it).second;
}

float & Line::y(size_t it)
{
    return m_coord->at(it).second;
}
