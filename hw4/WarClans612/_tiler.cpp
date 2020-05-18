// Developer: Wilbert (wilbert.phen@gmail.com)

#include "_matrix.hpp"
#include "_block.hpp"
#include "_tiler.hpp"

// default contructor
Tiler::Tiler(size_t N_row, size_t N_mid, size_t N_col)
    : NDIM_row(N_row), NDIM_mid(N_mid), NDIM_col(N_col), m_mat1(N_row, N_mid), m_mat2(N_col, N_mid)
{

}

void Tiler::load(
    Matrix const & mat1, size_t it1, size_t jt1,
    Matrix const & mat2, size_t it2, size_t jt2
)
{
    //const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    for (size_t i=0, base_t1=0, base_s1=it1*ncol1+jt1; i<NDIM_row; ++i, base_t1+=NDIM_mid, base_s1 += ncol1)
    {
        for (size_t j=0; j<NDIM_mid; ++j)
        {
            m_mat1(base_t1 + j) = mat1(base_s1 + j);
        }
    }

    //const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();
    for(size_t i=0, base_s2=it2*ncol2+jt2; i<NDIM_mid; ++i, base_s2 += ncol2)
    {
        for (size_t j=0, base_t2=0; j<NDIM_col; ++j, base_t2+=NDIM_mid)
        {
            m_mat2(base_t2 + i) = mat2(base_s2 + j);
        }
    }
}

void Tiler::multiply(Block &m_ret)
{
    double v;
    for (size_t i=0; i<NDIM_row; ++i)
    {
        for (size_t k=0; k<NDIM_col; ++k)
        {
            v = 0;
            for (size_t j=0; j<NDIM_mid; ++j)
            {
                v += m_mat1(i, j) * m_mat2(k, j);
            }
            m_ret(i, k) += v;
        }
    }
}
