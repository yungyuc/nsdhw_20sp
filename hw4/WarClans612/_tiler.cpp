// Developer: Wilbert (wilbert.phen@gmail.com)

#include "_matrix.hpp"
#include "_block.hpp"
#include "_tiler.hpp"

// default contructor
Tiler::Tiler(const Matrix &mat1, const Matrix &mat2, size_t tsize)
{
    //Load Matrix 1
    size_t init_b_mid=mat1.ncol()%tsize;
    size_t b_row=mat1.nrow()%tsize;
    for (size_t it=0; it<mat1.nrow(); it+=b_row, b_row=tsize)
    {
        std::vector<Block> temp;
        for (size_t jt=0, b_mid=init_b_mid; jt<mat1.ncol(); jt+=b_mid, b_mid=tsize)
        {
            temp.push_back(load1(mat1, it, jt, b_row, b_mid));
        }
        m_mat1.push_back(temp);
    }

    //Load Matrix 2
    size_t b_col=mat2.ncol()%tsize;
    for (size_t kt=0; kt<mat2.ncol(); kt+=b_col, b_col=tsize)
    {
        std::vector<Block> temp;
        for (size_t jt=0, b_mid=init_b_mid; jt<mat1.ncol(); jt+=b_mid, b_mid=tsize)
        {
            temp.push_back(load2(mat2, jt, kt, b_col, b_mid));
        }
        m_mat2.push_back(temp);
    }

    m_nrow = m_mat1.size();
    m_ncol = m_mat2.size();
    m_nmid = m_mat1.at(0).size();
}

Block Tiler::load1(Matrix const & mat1, size_t it1, size_t jt1, size_t NDIM_row, size_t NDIM_mid)
{
    //const size_t nrow1 = mat1.nrow();
    Block ret(NDIM_row, NDIM_mid);
    const size_t ncol1 = mat1.ncol();
    size_t base_s1=it1*ncol1+jt1;
    size_t base_t1=0;
    for (size_t i=0; i<NDIM_row; ++i)
    {
        for (size_t j=0; j<NDIM_mid; ++j)
        {
            ret(base_t1 + j) = mat1(base_s1 + j);
        }
        base_t1+=NDIM_mid;
        base_s1 += ncol1;
    }
    return ret;
}

Block Tiler::load2(Matrix const & mat2, size_t it2, size_t jt2, size_t NDIM_col, size_t NDIM_mid)
{
    //const size_t nrow2 = mat2.nrow();
    Block ret(NDIM_col, NDIM_mid);
    const size_t ncol2 = mat2.ncol();
    size_t base_s2=it2*ncol2+jt2;
    for(size_t i=0; i<NDIM_mid; ++i)
    {
        size_t base_t2=0;
        for (size_t j=0; j<NDIM_col; ++j)
        {
            ret(base_t2 + i) = mat2(base_s2 + j);
            base_t2+=NDIM_mid;
        }
        base_s2 += ncol2;
    }
    return ret;
}

void Tiler::multiply(Block &m_ret, size_t it, size_t jt, size_t kt)
{
    size_t ret_row = m_mat1[it][jt].nrow();
    size_t ret_mid = m_mat1[it][jt].ncol();
    Block left = m_mat1[it][jt];
    size_t ret_col = m_mat2[kt][jt].nrow();
    Block right = m_mat2[kt][jt];
    for (size_t i=0; i<ret_row; ++i)
    {
        for (size_t k=0; k<ret_col; ++k)
        {
            double v = 0;
            for (size_t j=0; j<ret_mid; ++j)
            {
                v += left(i, j) * right(k, j);
            }
            m_ret(i, k) += v;
        }
    }
}
