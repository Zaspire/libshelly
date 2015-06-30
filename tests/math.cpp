#include <gtest/gtest.h>

#include "math.hpp"

using namespace std;
using namespace shelly;

TEST(Math, General) {
  map<long long, int> t1{{3, 3}, {11, 1}, {13, 1}, {37, 1}};
  ASSERT_EQ(Factor(142857), t1);

  ASSERT_EQ(GreatestCommonDivisor(10, 37), 1);
  ASSERT_EQ(GreatestCommonDivisor(10, 38), 2);
}

TEST(Math, Matrix) {
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

TEST(Math, FastPower) {
  ASSERT_EQ(FastPower(2, 0), 1);
  ASSERT_EQ(FastPower(5, 1), 5);
  ASSERT_EQ(FastPower(2, 10), 1024);
  ASSERT_EQ(FastPower(5, 10), 9765625);

  ASSERT_EQ(FastPower(2LL, 53), 9007199254740992LL);

  ASSERT_EQ(FastPower(ModInt<int64_t, 1000000>(7), 1000).get(), 600001);
}
