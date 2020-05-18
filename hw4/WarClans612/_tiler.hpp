// Developer: Wilbert (wilbert.phen@gmail.com)

#pragma once
#include <iostream>
#include "_matrix.hpp"

class Tiler {

public:

    Tiler(size_t N_row, size_t N_mid, size_t N_col);

    ~Tiler() = default; // default destructor

    void load(
        Matrix const & mat1, size_t it1, size_t jt1, 
        Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply(Block &m_ret);

    size_t nrow() const { return NDIM_row; }
    size_t nmid() const { return NDIM_mid; }
    size_t ncol() const { return NDIM_col; }

private:

    size_t NDIM_row;
    size_t NDIM_mid;
    size_t NDIM_col;
    Block m_mat1; // row-major
    Block m_mat2; // column-major
    //Block m_ret; // row-major
};
