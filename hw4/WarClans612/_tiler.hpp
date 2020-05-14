// Developer: Wilbert (wilbert.phen@gmail.com)

#pragma once
#include <iostream>
#include "_matrix.hpp"

class Tiler {

public:

    Tiler(size_t N);

    ~Tiler() = default; // default destructor

    void load(
        Matrix const & mat1, size_t it1, size_t jt1, 
        Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply(Block &m_ret);

private:

    size_t NDIM;
    Block m_mat1; // row-major
    Block m_mat2; // column-major
    //Block m_ret; // row-major
};
