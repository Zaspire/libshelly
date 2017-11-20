#include <gtest/gtest.h>

#include "shelly/math/geometry.hpp"

using namespace std;
using namespace shelly;

TEST(Line, Check) {
  Line<int> l1(1, 3, -2, 0);
  ASSERT_TRUE(l1.Check(3, 5));
  ASSERT_FALSE(l1.Check(3, 4));
}
