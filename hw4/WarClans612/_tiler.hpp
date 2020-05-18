// Developer: Wilbert (wilbert.phen@gmail.com)

#pragma once
#include <iostream>
#include "_matrix.hpp"

class Tiler {

public:

    Tiler(const Matrix &mat1, const Matrix &mat2, size_t tsize);

    ~Tiler() = default; // default destructor

    Block load1(Matrix const & mat1, size_t it1, size_t jt1, size_t NDIM_row, size_t NDIM_mid);
    Block load2(Matrix const & mat2, size_t it2, size_t jt2, size_t NDIM_col, size_t NDIM_mid);
    void multiply(Block &m_ret, size_t it, size_t jt, size_t kt);

    size_t nrow() const { return m_nrow; }
    size_t nmid() const { return m_nmid; }
    size_t ncol() const { return m_ncol; }

private:

    size_t m_nrow;
    size_t m_ncol;
    size_t m_nmid;

    std::vector<std::vector<Block>> m_mat1; // row-major
    std::vector<std::vector<Block>> m_mat2; // column-major
};

