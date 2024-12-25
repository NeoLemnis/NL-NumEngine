#include <gtest/gtest.h>
#include "Matrix.h"

TEST(MatrixTest, MatrixMultiplication) {
    Matrix<2, 2, int> matrix1{{1, 0}, {0, 1}};
    matrix1 *= 2;
    EXPECT_EQ(matrix1(0, 0), 2);
    EXPECT_EQ(matrix1(0, 1), 0);
    EXPECT_EQ(matrix1(1, 0), 0);
    EXPECT_EQ(matrix1(1, 1), 2);

    Matrix<2, 2, int> matrix2{{1, 2}, {3, 4}};
    EXPECT_EQ(matrix2(0, 0), 1);
    EXPECT_EQ(matrix2(0, 1), 2);
    EXPECT_EQ(matrix2(1, 0), 3);
    EXPECT_EQ(matrix2(1, 1), 4);

    Matrix<2, 2, int> matrix3{};
    matrix3 = matrix1 * matrix2;
    EXPECT_EQ(matrix3(0, 0), 2);
    EXPECT_EQ(matrix3(0, 1), 4);
    EXPECT_EQ(matrix3(1, 0), 6);
    EXPECT_EQ(matrix3(1, 1), 8);
}

// Example test case for matrix addition
TEST(MatrixTest, MatrixAddition) {
    Matrix<2, 2, int> matrix1{{1, 2}, {3, 4}};
    Matrix<2, 2, int> matrix2{{5, 6}, {7, 8}};
    Matrix<2, 2, int> result = matrix1 + matrix2;

    EXPECT_EQ(result(0, 0), 6);
    EXPECT_EQ(result(0, 1), 8);
    EXPECT_EQ(result(1, 0), 10);
    EXPECT_EQ(result(1, 1), 12);
}