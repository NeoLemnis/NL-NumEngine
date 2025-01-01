#ifndef MATRIX_H
#define MATRIX_H

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

/**
 * @brief A class for a 2D matrix.
 *
 * @tparam T Type of the elements.
 */
template <typename T> class Matrix {
private:
  std::vector<std::vector<T>> data;
  size_t rows;
  size_t cols;

public:
  /**
   * @brief Default constructor.
   */
  Matrix() : rows(0), cols(0) {}

  /**
   * @brief Constructor with dimensions.
   *
   * @param rows Number of rows.
   * @param cols Number of columns.
   */
  Matrix(size_t rows, size_t cols)
      : rows(rows), cols(cols), data(rows, std::vector<T>(cols)) {}

  /**
   * @brief Constructor with initializer list.
   *
   * @param list Initializer list to initialize the matrix.
   */
  Matrix(std::initializer_list<std::initializer_list<T>> list)
      : rows(list.size()), cols(list.begin()->size()) {

    data.resize(rows);
    size_t rowIdx = 0;
    for (const auto &row : list) {
      if (row.size() != cols) {
        throw std::invalid_argument("Invalid number of columns");
      }
      data[rowIdx] = row;
      ++rowIdx;
    }
  }

  /**
   * @brief Constructor with vector of vectors.
   *
   * @param vec Vector of vectors to initialize the matrix.
   */
  Matrix(const std::vector<std::vector<T>> &vec)
      : rows(vec.size()), cols(vec.empty() ? 0 : vec[0].size()), data(vec) {}

  /**
   * @brief Access element at specified position.
   *
   * @param row Row index.
   * @param col Column index.
   * @return T& Reference to the element.
   * @throws std::out_of_range if the index is out of range.
   */
  auto operator()(size_t row, size_t col) -> T & {
    if (row >= rows || col >= cols) {
      throw std::out_of_range("Matrix index out of range");
    }
    return data[row][col];
  }

  /**
   * @brief Access element at specified position (const version).
   *
   * @param row Row index.
   * @param col Column index.
   * @return const T& Const reference to the element.
   * @throws std::out_of_range if the index is out of range.
   */
  auto operator()(size_t row, size_t col) const -> const T & {
    if (row >= rows || col >= cols) {
      throw std::out_of_range("Matrix index out of range");
    }
    return data[row][col];
  }

  /**
   * @brief Get the number of rows.
   *
   * @return size_t Number of rows.
   */
  [[nodiscard]] auto getRows() const -> size_t { return rows; }

  /**
   * @brief Get the number of columns.
   *
   * @return size_t Number of columns.
   */
  [[nodiscard]] auto getCols() const -> size_t { return cols; }

  auto operator+(const Matrix<T> &other) const -> Matrix<T> {
    if (rows != other.rows || cols != other.cols) {
      throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    Matrix<T> result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        result(i, j) = data[i][j] + other(i, j);
      }
    }
    return result;
  }

  auto operator-(const Matrix<T> &other) const -> Matrix<T> {
    if (rows != other.rows || cols != other.cols) {
      throw std::invalid_argument(
          "Matrix dimensions must match for subtraction");
    }
    Matrix<T> result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        result(i, j) = data[i][j] - other(i, j);
      }
    }
    return result;
  }

  auto operator*(const Matrix<T> &other) const -> Matrix<T> {
    // If the matrix or the other is a scalar
    if (cols == 1 && rows == 1) {
      Matrix<T> result(other.rows, other.cols);
      for (size_t i = 0; i < other.rows; ++i) {
        for (size_t j = 0; j < other.cols; ++j) {
          result(i, j) = data[0][0] * other(i, j);
        }
      }
      return result;
    }
    if (other.cols == 1 && other.rows == 1) {
      Matrix<T> result(rows, cols);
      for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
          result(i, j) = data[i][j] * other(0, 0);
        }
      }
      return result;
    }

    if (cols != other.rows) {
      throw std::invalid_argument(
          "Matrix dimensions must match for multiplication");
    }
    Matrix<T> result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < other.cols; ++j) {
        result(i, j) = 0;
        for (size_t k = 0; k < cols; ++k) {
          result(i, j) += data[i][k] * other(k, j);
        }
      }
    }
    return result;
  }

  auto operator*(T scalar) const -> Matrix<T> {
    Matrix<T> result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        result(i, j) = data[i][j] * scalar;
      }
    }
    return result;
  }

  // Left scalar multiplication
  friend auto operator*(T scalar, const Matrix<T> &matrix) -> Matrix<T> {
    return matrix * scalar;
  }

  // Scalar multiplication assignment
  auto operator*=(T scalar) -> Matrix<T> & {
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        data[i][j] *= scalar;
      }
    }
    return *this;
  }

  // Matrix addition assignment
  auto operator+=(const Matrix<T> &other) -> Matrix<T> & {
    validateDimensions(other, "addition");
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        data[i][j] += other(i, j);
      }
    }
    return *this;
  }

  // Matrix subtraction assignment
  auto operator-=(const Matrix<T> &other) -> Matrix<T> & {
    validateDimensions(other, "subtraction");
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        data[i][j] -= other(i, j);
      }
    }
    return *this;
  }

  friend auto operator<<(std::ostream &os, const Matrix<T> &matrix)
      -> std::ostream & {
    for (size_t i = 0; i < matrix.rows; ++i) {
      for (size_t j = 0; j < matrix.cols; ++j) {
        os << std::setw(8) << matrix(i, j);
      }
      os << '\n';
    }
    return os;
  }

private:
  void validateDimensions(const Matrix<T> &other,
                          const std::string &operation) const {
    if (rows != other.rows || cols != other.cols) {
      throw std::invalid_argument("Matrix dimensions must match for " +
                                  operation);
    }
  }
};

#endif // MATRIX_H
