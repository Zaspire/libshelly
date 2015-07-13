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

TEST(FindFirstTrue, General) {
  vector<int> a(1000);
  for (int i = 0; i < 1000; i++)
    a[i] = i;
  int v = FindFirstTrue(0, 999, [&a](int i) {
    return a[i] >= 400;
  });
  ASSERT_EQ(v, 400);

  v = FindFirstTrue(0, 999, [](int) {
    return true;
  });
  ASSERT_EQ(v, 0);

  v = FindFirstTrue(0, 999, [](int i) {
    return i >= 999;
  });
  ASSERT_EQ(v, 999);

  ASSERT_ANY_THROW({
    FindFirstTrue(0, 999, [](int i) {
      return i >= 2500;
    });
  });
}
