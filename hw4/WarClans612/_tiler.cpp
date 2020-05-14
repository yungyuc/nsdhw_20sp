// Developer: Wilbert (wilbert.phen@gmail.com)

#include "_matrix.hpp"
#include "_block.hpp"
#include "_tiler.hpp"

// default contructor
Tiler::Tiler(size_t N)
    : NDIM(N), m_mat1(N), m_mat2(N)
{

}

void Tiler::load(
    Matrix const & mat1, size_t it1, size_t jt1,
    Matrix const & mat2, size_t it2, size_t jt2
)
{
    //const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    //const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    for (size_t i=0, base_t1=0; i<NDIM; ++i, base_t1+=NDIM)
    {
        const size_t base_s1 = (it1 + i)*ncol1 + jt1;
        const size_t base_s2 = (it2 + i)*ncol2 + jt2;

        for (size_t j=0, base_t2=0; j<NDIM; ++j, base_t2+=NDIM)
        {
            m_mat1(base_t1 + j) = mat1(base_s1 + j);
            m_mat2(base_t2 + i) = mat2(base_s2 + j);
        }
    }
}

void Tiler::multiply(Block &m_ret)
{
    for (size_t i=0; i<NDIM; ++i)
    {
        for (size_t k=0; k<NDIM; ++k)
        {
            double v = 0;
            for (size_t j=0; j<NDIM; ++j)
            {
                v += m_mat1(i, j) * m_mat2(k, j);
            }
            m_ret(i, k) += v;
        }
    }
}