#include <gtest/gtest.h>

#include "shelly.hpp"
#include "fenwick_tree.hpp"

using namespace std;
using namespace shelly;

TEST(Shelly, General) {
  ASSERT_EQ(LongestCommonSubsequence("dax1czx23cxcz3ojli", "pomnk11vn23"), 3);

  vector<int> a{1, 2, 3};
  ASSERT_EQ(accumulate(ALL(a), 0), 6);
}

TEST(FenwickTree, General) {
  const int kMax = 1000;
  FenwickTree<int> tree(kMax);

  mt19937_64 _e1(chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(0, kMax - 1);
  vector<int> tt(kMax, 0);
  for (int i = 0; i < 2000; i++) {
    int t = dist(_e1);
    tree.add(t, i * i);
    tt[t] += i * i;

    t = dist(_e1);
    int ans = 0;
    for (int k = 0; k <= t; k++) {
      ans += tt[k];
    }
    ASSERT_EQ(tree.sum(t), ans);
  }
  int ans = 0;
  for (int i = 100; i <= 200; i++)
    ans += tt[i];
  ASSERT_EQ(tree.sum(100, 200), ans);
}
