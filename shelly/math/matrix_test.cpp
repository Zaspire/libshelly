#include "shelly/math/matrix.hpp"

#include <gtest/gtest.h>

using namespace std;
using namespace shelly;

TEST(Matrix, Multiplication) {
  Matrix<int> a(2, 1);
  a._data[0][0] = 2;
  a._data[1][0] = 1;
  Matrix<int> b(a.transpose());

  ASSERT_EQ(b._data[0].size(), 2U);
  ASSERT_EQ(b._data.size(), 1U);
  ASSERT_EQ(b._data[0][1], 1);

  Matrix<int> c = a * b;
  ASSERT_EQ(c._data[0].size(), 2U);
  ASSERT_EQ(c._data.size(), 2U);
  ASSERT_EQ(c._data[0][0], 4);
  ASSERT_EQ(c._data[1][1], 1);
}
