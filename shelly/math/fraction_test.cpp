#include <gtest/gtest.h>

#include "shelly/math/fraction.hpp"

using namespace shelly;

TEST(Fraction, General) {
  Fraction<int> a(1, 3), b(1, 2);

  ASSERT_EQ(Fraction<int>(5, 6), a + b);
  ASSERT_EQ(Fraction<int>(-1, 6), a - b);
  ASSERT_EQ(Fraction<int>(1, 6), a * b);

  ASSERT_EQ(Fraction<int>(5, 1), b * 10);

  ASSERT_NE(a, b);
}

TEST(Fraction, Division) {
  ASSERT_EQ(Fraction<int>(4, 1), Fraction<int>(1) / Fraction<int>(1, 4));
  ASSERT_EQ(Fraction<int>(2, 1), Fraction<int>(1, 2) / Fraction<int>(1, 4));
}
