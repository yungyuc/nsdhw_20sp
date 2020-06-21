// Developer: Wilbert (wilbert.phen@gmail.com)

#include "_matrix.hpp"
#include "_tiler.hpp"

// default contructor
Tiler::Tiler(const Matrix &mat1, const Matrix &mat2, size_t tsize)
{
    size_t rem_row1 = mat1.nrow()%tsize; //Remaining block size if not divisible by tsize
    size_t ret_row1 = mat1.nrow() - rem_row1; //Row count of tsize block
    size_t rem_col1 = mat1.ncol()%tsize; //Remaining block size if not divisible by tsize
    size_t ret_col1 = mat1.ncol() - rem_col1; //Column count of tsize block

    for (size_t it=0; it<ret_row1; it+=tsize)
    {
        std::vector<Matrix> temp;
        for (size_t jt=0; jt<ret_col1; jt+=tsize)
        {
            temp.push_back(load1(mat1, it, jt, tsize, tsize));
        }
        if (rem_col1 != 0)
        {
            temp.push_back(load1(mat1, it, ret_col1, tsize, rem_col1));
        }
        m_mat1.push_back(temp);
    }
    if (rem_row1 != 0)
    {
        std::vector<Matrix> temp;
        for (size_t jt=0; jt<ret_col1; jt+=tsize)
        {
            temp.push_back(load1(mat1, ret_row1, jt, rem_row1, tsize));
        }
        if (rem_col1 != 0)
        {
            temp.push_back(load1(mat1, ret_row1, ret_col1, rem_row1, rem_col1));
        }
        m_mat1.push_back(temp);
    }

    size_t rem_row2 = mat2.ncol()%tsize; //Remaining block size if not divisible by tsize
    size_t ret_row2 = mat2.ncol() - rem_row2; //Row count of tsize block
    size_t rem_col2 = mat1.ncol()%tsize; //Remaining block size if not divisible by tsize
    size_t ret_col2 = mat1.ncol() - rem_col2; //Column count of tsize block

    for (size_t it=0; it<ret_row2; it+=tsize)
    {
        std::vector<Matrix> temp;
        for (size_t jt=0; jt<ret_col2; jt+=tsize)
        {
            temp.push_back(load2(mat2, jt, it, tsize, tsize));
        }
        if (rem_col2 != 0)
        {
            temp.push_back(load2(mat2, ret_col2, it, tsize, rem_col2));
        }
        m_mat2.push_back(temp);
    }
    if (rem_row2 != 0)
    {
        std::vector<Matrix> temp;
        for (size_t jt=0; jt<ret_col2; jt+=tsize)
        {
            temp.push_back(load2(mat2, jt, ret_row2, rem_row2, tsize));
        }
        if (rem_col2 != 0)
        {
            temp.push_back(load2(mat2, ret_col2, ret_row2, rem_row2, rem_col2));
        }
        m_mat2.push_back(temp);
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

void Tiler::multiply(Matrix &m_ret, Matrix &left, Matrix &right)
{
    for (size_t i=0; i<left.nrow(); ++i)
    {
        for (size_t k=0; k<right.nrow(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<left.ncol(); ++j)
            {
                v += left(i, j) * right(k, j);
            }
            m_ret(i, k) += v;
        }
    }
}