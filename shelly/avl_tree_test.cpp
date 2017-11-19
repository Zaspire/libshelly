#include <gtest/gtest.h>

#include "avl_tree.hpp"
#include <random>

using namespace std;
using namespace shelly;

TEST(AVLTree, Insert) {
  {
  AVLTree<int, int> a;

  for (int i = 0; i < 1000; i++) {
    a.Insert(i, i * i);
    for (int k = 0; k <= i; k++) {
      ASSERT_EQ(a.Find(k).first, k * k);
      ASSERT_TRUE(a.Find(k).second);
    }
  }
  }
  {
  AVLTree<int, int> a;

  for (int i = 100; i > 0; i--) {
    a.Insert(i, i * i);
    for (int k = i; k <= 100; k++)
      ASSERT_EQ(a.Find(k).first, k * k);
  }
  }
  AVLTree<int, int> b;
  mt19937_64 _e1(chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(0, 1000000);
  for (int i = 0; i < 1000; i++) {
    int t = dist(_e1);
    b[t] = i * i;
    ASSERT_EQ(b.Find(t).first, i * i);
  }
}

TEST(AVLTree, FuzzyDeletion) {
  AVLTree<int, int> b;
  map<int, int> rb;
  mt19937_64 _e1(chrono::system_clock::now().time_since_epoch().count());
  std::uniform_int_distribution<int> dist(0, 1000);
  for (int i = 0; i < 5000; i++) {
    int t = dist(_e1);
    if (i % 3 == 0) {
      auto it = rb.find(t);
      if (it == rb.end()) {
        ASSERT_EQ(b.Erase(t), false);
      } else {
        rb.erase(t);
        ASSERT_EQ(b.Erase(t), true);
        ASSERT_EQ(b.Find(t).second, false);
      }
    }
    b[t] = i * i;
    rb[t] = i * i;
    ASSERT_EQ(b.Find(t).first, i * i);
  }
  ASSERT_EQ(b.Size(), rb.size());
  for (int i = 0; i < 1000; i++) {
    auto it = rb.find(i);
    if (it == rb.end()) {
      ASSERT_EQ(b.Find(i).second, false);
      ASSERT_EQ(b.Erase(i), false);
      continue;
    }
    ASSERT_EQ(b.Find(i).second, true);
    ASSERT_EQ(b.Find(i).first, rb[i]);

    rb.erase(i);
    ASSERT_EQ(b.Erase(i), true);
    ASSERT_EQ(b.Find(i).second, false);
  }
  ASSERT_EQ(b.Size(), rb.size());
}
