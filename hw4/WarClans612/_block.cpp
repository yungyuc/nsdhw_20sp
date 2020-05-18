// Developer: Wilbert (wilbert.phen@gmail.com)

#include "_block.hpp"

// default contructor
Block::Block(size_t N_row, size_t N_col)
    : NDIM_row(N_row), NDIM_col(N_col), m_buffer(N_row * N_col, 0)
{
    std::fill(m_buffer.begin(), m_buffer.end(), 0);
}

Block& Block::operator= (double v)
{
    std::fill(m_buffer.begin(), m_buffer.end(), v);
    return *this;
}

Block& Block::operator+= (Block const &other)
{
    const size_t max_i = NDIM_row*NDIM_col;
    for (size_t i=0; i<max_i; ++i) { m_buffer.at(i) += other.m_buffer.at(i); }
    return *this;
}

void Block::save(Matrix &mat, size_t it, size_t jt)
{
    const size_t ncol = mat.ncol();
    size_t base_s;
    size_t base_t;

    for (size_t i=0, base_s=0, base_t=it*ncol+jt; i<NDIM_row; ++i, base_s+=NDIM_col, base_t+=ncol)
    {
        for (size_t j=0; j<NDIM_col; ++j)
        {
            mat(base_t + j) = m_buffer.at(base_s + j);
        }
    }
}