#include <gtest/gtest.h>

#include "shelly/math/math.hpp"

using namespace std;
using namespace shelly;

TEST(Math, General) {
  map<int64_t, int> t1{{3, 3}, {11, 1}, {13, 1}, {37, 1}};
  ASSERT_EQ(Factor(142857), t1);

  ASSERT_EQ(GreatestCommonDivisor(10, 37), 1);
  ASSERT_EQ(GreatestCommonDivisor(10, 38), 2);
}

TEST(Math, FastPower) {
  ASSERT_EQ(FastPower(2, 0), 1);
  ASSERT_EQ(FastPower(5, 1), 5);
  ASSERT_EQ(FastPower(2, 10), 1024);
  ASSERT_EQ(FastPower(5, 10), 9765625);

  ASSERT_EQ(FastPower(2LL, 53), 9007199254740992LL);
}
