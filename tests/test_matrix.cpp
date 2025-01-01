#include "Matrix.h"
#include <gtest/gtest.h>

TEST(MatrixTest, Construction) {
  Matrix<double> m1(2, 2);
  EXPECT_EQ(m1.getRows(), 2);
  EXPECT_EQ(m1.getCols(), 2);

  Matrix<double> m2{{1, 2}, {3, 4}};
  EXPECT_EQ(m2(0, 0), 1);
  EXPECT_EQ(m2(0, 1), 2);
  EXPECT_EQ(m2(1, 0), 3);
  EXPECT_EQ(m2(1, 1), 4);
}

TEST(MatrixTest, Addition) {
  Matrix<double> m1{{1, 2}, {3, 4}};
  Matrix<double> m2{{5, 6}, {7, 8}};
  Matrix<double> result = m1 + m2;

  EXPECT_EQ(result(0, 0), 6);
  EXPECT_EQ(result(0, 1), 8);
  EXPECT_EQ(result(1, 0), 10);
  EXPECT_EQ(result(1, 1), 12);
}

TEST(MatrixTest, Multiplication) {
  Matrix<double> m1{{1, 2}, {3, 4}};
  Matrix<double> m2{{5, 6}, {7, 8}};
  Matrix<double> result = m1 * m2;

  EXPECT_EQ(result(0, 0), 19);
  EXPECT_EQ(result(0, 1), 22);
  EXPECT_EQ(result(1, 0), 43);
  EXPECT_EQ(result(1, 1), 50);
}

TEST(MatrixTest, ScalarMultiplication) {
  Matrix<double> m{{1, 2}, {3, 4}};
  Matrix<double> result = m * 2;

  EXPECT_EQ(result(0, 0), 2);
  EXPECT_EQ(result(0, 1), 4);
  EXPECT_EQ(result(1, 0), 6);
  EXPECT_EQ(result(1, 1), 8);
}