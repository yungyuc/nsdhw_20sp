// Developer: Wilbert (wilbert.phen@gmail.com)

#include "_block.hpp"

// default contructor
Block::Block(size_t N)
    : NDIM(N), m_buffer(N * N, 0)
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
    for (size_t i=0; i<NDIM*NDIM; ++i) { m_buffer.at(i) += other.m_buffer.at(i); }
    return *this;
}

void Block::save(Matrix &mat, size_t it, size_t jt)
{
    const size_t ncol = mat.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s = i*NDIM;
        const size_t base_t = (it + i)*ncol + jt;

        for (size_t j=0; j<NDIM; ++j)
        {
            mat(base_t + j) += m_buffer.at(base_s + j);
        }
    }
}