#include <gtest/gtest.h>

#include "algorithm.hpp"

using namespace std;
using namespace shelly;

TEST(LongestIncreaseSequence, General) {
  ASSERT_EQ(LongestIncreaseSequence(vector<int>{1, 5, 2, 3, 1, 6}), 4);

  vector<int64_t> t;
  for (int i = -100; i < 100; i++)
    t.push_back(i);
  ASSERT_EQ(LongestIncreaseSequence(t), 200);
  ASSERT_EQ(LongestIncreaseSequence(vector<int64_t>(t.rbegin(), t.rend())), 1);
}

TEST(LongestCommonSubsequence, General) {
  ASSERT_EQ(LongestCommonSubsequence("dax1czx23cxcz3ojli", "pomnk11vn23"), 3);

  string t;
  for (char c = 'a'; c <= 'z'; c++)
    t.push_back(c);

  ASSERT_EQ(LongestCommonSubsequence(t, t), int(t.size()));
  ASSERT_EQ(LongestCommonSubsequence(t, string(t.rbegin(), t.rend())), 1);
}
