// Developer: Wilbert (wilbert.phen@gmail.com)

#include "_matrix.hpp"
#include "_tiler.hpp"

// default contructor
Tiler::Tiler(const Matrix &mat1, const Matrix &mat2, size_t tsize)
    : m_mat1(mat1.nrow()/tsize + 1), m_mat2(mat2.ncol()/tsize + 1)
{
    //Load Matrix 1
    size_t b_row=mat1.nrow()%tsize;
    size_t mem_reserved = mat1.ncol()/tsize+1;
    for (size_t it=0, id=0; it<mat1.nrow(); it+=b_row, ++id, b_row=tsize)
    {
        m_mat1[id].reserve(mem_reserved);
        for (size_t jt=0, b_mid=mat1.ncol()%tsize; jt<mat1.ncol(); jt+=b_mid, b_mid=tsize)
        {
            m_mat1[id].push_back(load1(mat1, it, jt, b_row, b_mid));
        }
    }

    //Load Matrix 2
    size_t b_col=mat2.ncol()%tsize;
    for (size_t kt=0, id=0; kt<mat2.ncol(); kt+=b_col, ++id, b_col=tsize)
    {
        m_mat2[id].reserve(mem_reserved);
        for (size_t jt=0, b_mid=mat1.ncol()%tsize; jt<mat1.ncol(); jt+=b_mid, b_mid=tsize)
        {
            m_mat2[id].push_back(load2(mat2, jt, kt, b_col, b_mid));
        }
    }

    m_nrow = m_mat1.size();
    m_ncol = m_mat2.size();
    m_nmid = m_mat1.at(0).size();
}

Matrix Tiler::load1(Matrix const & mat1, size_t it1, size_t jt1, size_t NDIM_row, size_t NDIM_mid)
{
    //const size_t nrow1 = mat1.nrow();
    Matrix ret(NDIM_row, NDIM_mid);
    const size_t ncol1 = mat1.ncol();
    for (size_t i=0; i<NDIM_row; ++i)
    {
        size_t base_s1=(it1+i)*ncol1+jt1;
        size_t base_t1=i*NDIM_mid;
        for (size_t j=0; j<NDIM_mid; ++j)
        {
            ret(base_t1 + j) = mat1(base_s1 + j);
        }
    }
    return ret;
}

Matrix Tiler::load2(Matrix const & mat2, size_t it2, size_t jt2, size_t NDIM_col, size_t NDIM_mid)
{
    //const size_t nrow2 = mat2.nrow();
    Matrix ret(NDIM_col, NDIM_mid);
    const size_t ncol2 = mat2.ncol();
    for(size_t i=0; i<NDIM_mid; ++i)
    {
        size_t base_s2=(it2+i)*ncol2+jt2;
        for (size_t j=0; j<NDIM_col; ++j)
        {
            ret(j*NDIM_mid + i) = mat2(base_s2 + j);
        }
    }
    return ret;
}

void Tiler::multiply(Matrix &m_ret, size_t it, size_t jt, size_t kt)
{
    Matrix* left = &m_mat1[it][jt];
    Matrix* right = &m_mat2[kt][jt];

    for (size_t i=0; i<left->nrow(); ++i)
    {
        for (size_t k=0; k<right->nrow(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<left->ncol(); ++j)
            {
                v += (*left)(i, j) * (*right)(k, j);
            }
            m_ret(i, k) += v;
        }
    }
}
