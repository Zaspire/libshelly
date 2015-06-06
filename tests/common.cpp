#include <gtest/gtest.h>

#include "math.hpp"
#include "shelly.hpp"
#include "avl_tree.hpp"

using namespace std;
using namespace shelly;

TEST(Math, General) {
  map<long long, int> t1{{3, 3}, {11, 1}, {13, 1}, {37, 1}};
  ASSERT_EQ(Factor(142857), t1);

  ASSERT_EQ(GreatestCommonDivisor(10, 37), 1);
  ASSERT_EQ(GreatestCommonDivisor(10, 38), 2);

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

TEST(Shelly, General) {
  ASSERT_EQ(LongestCommonSubsequence("dax1czx23cxcz3ojli", "pomnk11vn23"), 3);

  vector<int> a{1, 2, 3};
  ASSERT_EQ(accumulate(ALL(a), 0), 6);
}

TEST(AVLTree, General) {
  {
  AVLTree<int, int> a;

  for (int i = 0; i < 1000; i++) {
    a.insert(i, i * i);
    for (int k = 0; k <= i; k++)
      ASSERT_EQ(a.Find(k), k * k);
  }
  }
  {
  AVLTree<int, int> a;

  for (int i = 100; i > 0; i--) {
    a.insert(i, i * i);
    for (int k = i; k <= 100; k++)
      ASSERT_EQ(a.Find(k), k * k);
  }
  }
  /*  AVLTree<int, int> b;
  mt19937_64 _e1(chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(0, 1000000);
  for (int i = 0; i < 1000; i++) {
    int t = dist(_e1);
    b.insert(t, i * i);
    ASSERT_EQ(b.Find(t), i * i);
    }*/
}
