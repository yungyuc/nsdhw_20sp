// Developer: Wilbert (wilbert.phen@gmail.com)

#pragma once
#include <iostream>
#include <vector>
#include "_matrix.hpp"

class Block {

public:

    Block(size_t N_row, size_t N_col);

    ~Block() = default; // default destructor

    double   operator() (size_t row, size_t col) const { return m_buffer.at( row*NDIM_col + col ); }
    double & operator() (size_t row, size_t col)       { return m_buffer.at( row*NDIM_col + col ); }
    double   operator() (size_t idx) const { return m_buffer.at( idx ); }
    double & operator() (size_t idx)       { return m_buffer.at( idx ); }

    Block &operator= (double v);
    Block &operator+= (Block const &other);

    void save(Matrix &mat, size_t it, size_t jt);

    size_t nrow() const { return NDIM_row; }
    size_t ncol() const { return NDIM_col; }

private:

    size_t NDIM_row;
    size_t NDIM_col;
public:
    std::vector<double> m_buffer;
};