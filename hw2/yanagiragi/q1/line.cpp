#include "line.hpp"

Line::Line(size_t size) {
    m_coord.resize(size * 2);
}

// copy constructor
Line::Line(Line const & other) {
    m_coord.clear();
    m_coord.assign(other.m_coord.begin(), other.m_coord.end());
}

// move constructor
Line::Line(Line &&other) {
    std::swap(other.m_coord, m_coord);
}

// copy assignment 
Line& Line::operator=(Line const &other) {
    if (this == &other) {
        return *this;
    }
    m_coord.clear();
    m_coord.assign(other.m_coord.begin(), other.m_coord.end());
    return *this;
}

// move assignment 
Line& Line::operator=(Line &&other) {
    if (this == &other) {
        return *this;
    }
    std::swap(other.m_coord, m_coord);
    return *this;
}

Line::~Line() {
    m_coord.clear();
}

size_t Line::size() const {
    return m_coord.size() / 2;
}

void Line::check_range(size_t it) const {
    if (it >= m_coord.size()) {
        throw std::out_of_range("Line index out of range"); 
    }
}

float & Line::x(size_t it) {
    check_range(it);
    return m_coord[it * 2];
}

float const & Line::x(size_t it) const {
    check_range(it);
    return m_coord[it * 2];
}

float & Line::y(size_t it) {
    check_range(it);
    return m_coord[it * 2 + 1];
}

float const & Line::y(size_t it) const {
    check_range(it);
    return m_coord[it * 2 + 1];
}