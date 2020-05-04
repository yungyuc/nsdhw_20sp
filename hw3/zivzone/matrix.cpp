#include <mkl.h>
#include <pybind11/pybind11.h>
#include <vector>
//namespace py = pybind11;

class Matrix {
public:
  Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), matrix_data(nrow * ncol) 
      {
        //  reset_buffer(nrow, ncol);
      }
  Matrix(const Matrix &) = default;
  Matrix(Matrix &&) = default;
  Matrix &operator=(const Matrix &) = default;
  Matrix &operator=(Matrix &&) = default;
  ~Matrix() = default;
    // {
    //     reset_buffer(0, 0);
    // }

  double   operator() (size_t i, size_t j) const { if (i >= m_nrow || j >= m_ncol) throw pybind11::index_error(); return matrix_data[i * m_ncol + j];}


  double & operator() (size_t i, size_t j)       { if (i >= m_nrow || j >= m_ncol) throw pybind11::index_error(); return matrix_data[i * m_ncol + j];}

  size_t nrow() const { return m_nrow; }
  size_t ncol() const { return m_ncol; }

  bool operator == (const Matrix &second_matrix) const {
    if (m_nrow == second_matrix.m_nrow){
      if (m_ncol == second_matrix.m_ncol){
        if (matrix_data == second_matrix.matrix_data){
          return true; 
        }else{
          return false;
        }

      }else{
          return false;
      }
    }else{
          return false;
    }
  }
  bool operator != (const Matrix &second_matrix) const { return !(*this == second_matrix); }
  size_t size() const { return m_nrow * m_ncol; }
  double buffer(size_t i) const { return m_buffer[i]; }
  std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

  friend Matrix multiply_naive(const Matrix &, const Matrix &);
  friend Matrix multiply_mkl(const Matrix &, const Matrix &);

private:
  size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

  void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { delete[] m_buffer; }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = new double[nelement]; }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }
  size_t m_nrow = 0;
  size_t m_ncol = 0;
  double * m_buffer = nullptr;

  std::vector<double> matrix_data;
};

/*
 * Naive matrix matrix multiplication.
 */
 /* 
Matrix operator*(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        ostr << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            ostr << " " << std::setw(2) << mat(i, j);
        }
    }

    return ostr;
}
*/

Matrix multiply_naive(const Matrix &first_matrix, const Matrix &second_matrix) {
  const size_t f_row = first_matrix.m_nrow;
  const size_t f_col = first_matrix.m_ncol;
  const size_t s_col = second_matrix.m_ncol;
  Matrix result_matrix(f_row, s_col);
  std::fill(std::begin(result_matrix.matrix_data), std::end(result_matrix.matrix_data), 0);
  for (size_t i = 0; i < f_row; ++i){
    for (size_t j = 0; j < s_col; ++j){
      for (size_t k = 0; k < f_col; ++k){
          result_matrix(i, j) += first_matrix(i, k) * second_matrix(k, j);
      }
    }
  }
    
  return result_matrix;
}

Matrix multiply_mkl(const Matrix &first_matrix, const Matrix &second_matrix) {
  size_t f_nrow = first_matrix.nrow();
  size_t s_ncol = second_matrix.ncol();

  //Matrix result_matrix(first_matrix.nrow(), second_matrix.ncol());
  Matrix result_matrix(f_nrow, s_ncol);

  std::fill(std::begin(result_matrix.matrix_data), std::end(result_matrix.matrix_data), 0);
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
              first_matrix.nrow(), second_matrix.ncol(),first_matrix.ncol(), 1., first_matrix.matrix_data.data(), 
              first_matrix.ncol(), second_matrix.matrix_data.data(), second_matrix.ncol(), 1., 
              result_matrix.matrix_data.data(), result_matrix.ncol());
  return result_matrix;
}


/* 
int main(int argc, char ** argv)
{
    std::cout << ">>> A(2x3) times B(3x2):" << std::endl;
    Matrix mat1(2, 3, std::vector<double>{1, 2, 3, 4, 5, 6});
    Matrix mat2(3, 2, std::vector<double>{1, 2, 3, 4, 5, 6});

    Matrix mat3 = mat1 * mat2;

    std::cout << "matrix A (2x3):" << mat1 << std::endl;
    std::cout << "matrix B (3x2):" << mat2 << std::endl;
    std::cout << "result matrix C (2x2) = AB:" << mat3 << std::endl;

    std::cout << ">>> B(3x2) times A(2x3):" << std::endl;
    Matrix mat4 = mat2 * mat1;
    std::cout << "matrix B (3x2):" << mat2 << std::endl;
    std::cout << "matrix A (2x3):" << mat1 << std::endl;
    std::cout << "result matrix D (3x3) = BA:" << mat4 << std::endl;

    return 0;
}

// vim: set ff=unix fenc=utf8 et sw=4 ts=4 sts=4:
*/

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "class Matrix with  mkl & navie mutply";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_mkl", &multiply_mkl);
  pybind11::class_<Matrix>(m, "Matrix")
      .def(pybind11::init<size_t, size_t>())
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__getitem__", [](const Matrix &m, std::pair<size_t, size_t> it) {return m(it.first, it.second); })
      .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> it, double vec) { m(it.first, it.second) = vec; })
      .def("__eq__"     , [](const Matrix &first_matrix, const Matrix &second_matrix) { return first_matrix == second_matrix; })
      .def("__ne__"     , [](const Matrix &first_matrix, const Matrix &second_matrix) { return first_matrix != second_matrix; })
      ;
}
