#include <gtest/gtest.h>

#include "shelly/math/mod_int.hpp"
#include "shelly/math/math.hpp"

using namespace std;
using namespace shelly;

TEST(ModInt, FastPower) {
  ASSERT_EQ(FastPower(ModInt<int64_t, 1000000>(7), 1000).get(), 600001);
}

TEST(ModInt, ToInt64) {
  ModInt<int, 5> a(3);

  ASSERT_EQ(4, ToInt64(a * 2 - 2));
}
