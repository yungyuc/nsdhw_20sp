#include <mkl.h>
//#include <pybind11/pybind11.h>
#include <vector>


class Matrix {
public:
  Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol), matrix_data(nrow * ncol) {}
  Matrix(const Matrix &) = default;
  Matrix(Matrix &&) = default;
  Matrix &operator=(const Matrix &) = default;
  Matrix &operator=(Matrix &&) = default;
  ~Matrix() = default;

  double   operator() (size_t i, size_t j) const { if (i >= m_nrow || j >= m_ncol) throw pybind11::index_error(); return matrix_data[i * m_ncol + j];}


  double & operator() (size_t i, size_t j)       { if (i >= m_nrow || j >= m_ncol) throw pybind11::index_error(); return matrix_data[i * m_ncol + j];}

  constexpr size_t nrow() const { return m_nrow; }
  constexpr size_t ncol() const { return m_ncol; }

  constexpr bool operator == (const Matrix &second_matrix) const {
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
  constexpr bool operator != (const Matrix &second_matrix) const { return !(*this == second_matrix); }
  size_t size() const { return m_nrow * m_ncol; }
  double buffer(size_t i) const { return m_buffer[i]; }
  std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }
  
  friend Matrix multiply_naive(const Matrix &, const Matrix &);
  friend Matrix multiply_mkl(const Matrix &, const Matrix &);
  friend Matrix multiply_tile(const Matrix &, const Matrix &, size_t);


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
  
  Matrix result_matrix(f_nrow, s_ncol);

  std::fill(std::begin(result_matrix.matrix_data), std::end(result_matrix.matrix_data), 0);
  
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
              first_matrix.nrow(), second_matrix.ncol(),first_matrix.ncol(), 1., first_matrix.matrix_data.data(), 
              first_matrix.ncol(), second_matrix.matrix_data.data(), second_matrix.ncol(), 1., 
              result_matrix.matrix_data.data(), result_matrix.ncol());
  return result_matrix;
}


constexpr size_t tile_cur_value(int matrix_s, int tile_v) {
  size_t diff = ((matrix_s - tile_v) & (matrix_s - tile_v) >> 31);
  return tile_v + diff;
}

Matrix multiply_tile(const Matrix &first_matrix, const Matrix &second_matrix, size_t tile) {
  const size_t f_row = first_matrix.m_nrow;
  const size_t f_col = first_matrix.m_ncol;
  const size_t s_col = second_matrix.m_ncol;
  Matrix result_matrix(f_row, s_col);
  std::fill(std::begin(result_matrix.matrix_data), std::end(result_matrix.matrix_data), 0);
  const double *first_matrix_data  = first_matrix.matrix_data.data();
  const double *second_matrix_data = second_matrix.matrix_data.data();
  double *result_matrix_data       = result_matrix.matrix_data.data();

  for (size_t i_tile = 0; i_tile < f_row; i_tile += tile) {
    for (size_t j_tile = 0; j_tile < s_col; j_tile += tile) {
      for (size_t k_tile = 0; k_tile < f_col; k_tile += tile) {
        //const size_t i_tile_cur = f_row + ((i_tile + tile - f_row) & (i_tile + tile - f_row) >> 31);
        const size_t i_tile_cur = tile_cur_value((i_tile + tile),f_row);
        //const size_t j_tile_cur = s_col + ((j_tile + tile - s_col) & (j_tile + tile - s_col) >> 31);
        const size_t j_tile_cur = tile_cur_value((j_tile + tile),s_col);
        //const size_t k_tile_cur = f_col + ((k_tile + tile - f_col) & (k_tile + tile - f_col) >> 31);
        const size_t k_tile_cur = tile_cur_value((k_tile + tile),f_col);
        for (size_t i = i_tile; i < i_tile_cur; ++i) {
          const size_t index = i * f_row;
          for (size_t j = j_tile; j < j_tile_cur; ++j) {
            double partial_ans = 0;
            for (size_t k = k_tile; k < k_tile_cur; ++k) {
              partial_ans += first_matrix_data[index + k] * second_matrix_data[k * f_col + j];
            }
            result_matrix_data[index + j] += partial_ans;
          }
        }
      }
    }
  }    
  return result_matrix;
}

/*PYBIND11_MODULE(_matrix, m) {
  m.doc() = "class Matrix with  mkl & navie mutply";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_mkl", &multiply_mkl);
  m.def("multiply_tile", &multiply_tile);
  pybind11::class_<Matrix>(m, "Matrix")
      .def(pybind11::init<size_t, size_t>())
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__getitem__", [](const Matrix &m, std::pair<size_t, size_t> it) {return m(it.first, it.second); })
      .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> it, double vec) { m(it.first, it.second) = vec; })
      .def("__eq__"     , [](const Matrix &first_matrix, const Matrix &second_matrix) { return first_matrix == second_matrix; })
      .def("__ne__"     , [](const Matrix &first_matrix, const Matrix &second_matrix) { return first_matrix != second_matrix; })
      ;
}*/
