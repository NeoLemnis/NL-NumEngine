#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <iostream>

/**
 * @brief A template class for a 2D matrix.
 *
 * @tparam m Number of rows.
 * @tparam n Number of columns.
 * @tparam T Type of the elements.
 */
template <size_t m, size_t n, typename T>
class Matrix
{
private:
    std::array<std::array<T, n>, m> data; ///< 2D array to store matrix elements.

public:
    /**
     * @brief Default constructor.
     */
    Matrix() = default;

    /**
     * @brief Constructor with initializer list.
     *
     * @param list Initializer list to initialize the matrix.
     */
    Matrix(std::initializer_list<std::initializer_list<T>> list)
    {
        if (list.size() != m)
        {
            throw std::invalid_argument("Invalid number of rows");
        }
        size_t rowIdx = 0;
        for (const auto &row : list)
        {
            if (row.size() != n)
            {
                throw std::invalid_argument("Invalid number of columns");
            }
            size_t colIdx = 0;
            for (const auto &elem : row)
            {
                data.at(rowIdx).at(colIdx) = elem;
                ++colIdx;
            }
            ++rowIdx;
        }
    }

    /**
     * @brief Access element at specified position.
     *
     * @param row Row index.
     * @param col Column index.
     * @return T& Reference to the element.
     * @throws std::out_of_range if the index is out of range.
     */
    auto operator()(size_t row, size_t col) -> T &
    {
        if (row >= m || col >= n)
        {
            throw std::out_of_range("Matrix index out of range");
        }
        return data.at(row).at(col);
    }

    /**
     * @brief Access element at specified position (const version).
     *
     * @param row Row index.
     * @param col Column index.
     * @return const T& Const reference to the element.
     * @throws std::out_of_range if the index is out of range.
     */
    auto operator()(size_t row, size_t col) const -> const T &
    {
        if (row >= m || col >= n)
        {
            throw std::out_of_range("Matrix index out of range");
        }
        return data.at(row).at(col);
    }

    /**
     * @brief Get the number of rows.
     *
     * @return constexpr size_t Number of rows.
     */
    [[nodiscard]] constexpr auto rows() const noexcept -> size_t
    {
        return m;
    }

    /**
     * @brief Get the number of columns.
     *
     * @return constexpr size_t Number of columns.
     */
    [[nodiscard]] constexpr auto cols() const noexcept -> size_t
    {
        return n;
    }

    /**
     * @brief Fill the matrix with a specified value.
     *
     * @param value Value to fill the matrix with.
     */
    void fill(const T &value)
    {
        for (auto &row : data)
        {
            row.fill(value);
        }
    }

    /**
     * @brief Scalar multiplication of the matrix.
     *
     * @param scalar The scalar value.
     * @return Matrix<m, n, T> The result of the scalar multiplication.
     */
    auto operator*(const T &scalar) const -> Matrix<m, n, T>
    {
        Matrix<m, n, T> result{};
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                result(i, j) = data[i][j] * scalar;
            }
        }
        return result;
    }

    /**
     * @brief Scalar multiplication of the matrix.
     *
     * @param scalar The scalar value.
     * @return Matrix<m, n, T>& Reference to this matrix.
     */
    auto operator*=(const T &scalar) -> Matrix<m, n, T> &
    {
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                data.at(i).at(j) *= scalar;
            }
        }
        return *this;
    }

    /**
     * @brief Add two matrices.
     *
     * @param other The matrix to add.
     * @return Matrix<m, n, T> The result of the addition.
     */
    auto operator+(const Matrix<m, n, T> &other) const -> Matrix<m, n, T>
    {
        Matrix<m, n, T> result{};
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                result(i, j) = data.at(i).at(j) + other(i, j);
            }
        }
        return result;
    }

    /**
     * @brief Add another matrix to this matrix.
     *
     * @param other The matrix to add.
     * @return Matrix<m, n, T>& Reference to this matrix.
     */
    auto operator+=(const Matrix<m, n, T> &other) -> Matrix<m, n, T> &
    {
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                data[i][j] += other(i, j);
            }
        }
        return *this;
    }

    /**
     * @brief Subtract one matrix from another.
     *
     * @param other The matrix to subtract.
     * @return Matrix<m, n, T> The result of the subtraction.
     */
    auto operator-(const Matrix<m, n, T> &other) const -> Matrix<m, n, T>
    {
        Matrix<m, n, T> result{};
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                result(i, j) = data[i][j] - other(i, j);
            }
        }
        return result;
    }

    /**
     * @brief Subtract another matrix from this matrix.
     *
     * @param other The matrix to subtract.
     * @return Matrix<m, n, T>& Reference to this matrix.
     */
    auto operator-=(const Matrix<m, n, T> &other) -> Matrix<m, n, T> &
    {
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                data[i][j] -= other(i, j);
            }
        }
        return *this;
    }

    /**
     * @brief Multiply two matrices.
     *
     * @tparam p Number of columns in the other matrix.
     * @param other The matrix to multiply with.
     * @return Matrix<m, p, T> The result of the multiplication.
     */
    template <size_t p>
    auto operator*(const Matrix<n, p, T> &other) const -> Matrix<m, p, T>
    {
        Matrix<m, p, T> result{};
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < p; ++j)
            {
                for (size_t k = 0; k < n; ++k)
                {
                    result(i, j) += data.at(i).at(k) * other(k, j);
                }
            }
        }
        return result;
    }

    /**
     * @brief Multiply this matrix by another matrix.
     *
     * @tparam p Number of columns in the other matrix.
     * @param other The matrix to multiply with.
     * @return Matrix<m, p, T>& Reference to this matrix.
     */
    template <size_t p>
    auto operator*=(const Matrix<n, p, T> &other) -> Matrix<m, p, T> &
    {
        *this = *this * other;
        return *this;
    }

    /**
     * @brief Compare two matrices for equality.
     *
     * @param other The matrix to compare with.
     * @return bool True if the matrices are equal, false otherwise.
     */
    auto operator==(const Matrix<m, n, T> &other) const -> bool
    {
        return data == other.data;
    }

    /**
     * @brief Compare two matrices for inequality.
     *
     * @param other The matrix to compare with.
     * @return bool True if the matrices are not equal, false otherwise.
     */
    auto operator!=(const Matrix<m, n, T> &other) const -> bool
    {
        return !(*this == other);
    }

    /**
     * @brief Get the matrix data as a 2D array.
     *
     * @return std::array<std::array<T, n>, m> The matrix data.
     * @note This function is mainly for testing purposes.
     */
    [[nodiscard]] auto to_array() const -> std::array<std::array<T, n>, m>
    {
        return data;
    }

    /**
     * @brief Output the matrix to an output stream.
     *
     * @param outStream The output stream.
     * @param matrix The matrix to output.
     * @return std::ostream& The output stream.
     */
    friend auto operator<<(std::ostream &outStream, const Matrix &matrix) -> std::ostream &
    {
        for (const auto &row : matrix.data)
        {
            for (const auto &elem : row)
            {
                outStream << elem << ' ';
            }
            outStream << '\n';
        }
        return outStream;
    }
};

#endif // MATRIX_H
