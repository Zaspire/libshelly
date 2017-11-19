#include <gtest/gtest.h>

#include "string_utils.hpp"

using namespace std;
using namespace shelly;

TEST(StringUtils, Split) {
  ASSERT_EQ(Split(" 1 2   3", ' '), (vector<string>{"1", "2", "3"}));
  ASSERT_EQ(Split("1    2   3 4 ", ' '), (vector<string>{"1", "2", "3", "4"}));
  ASSERT_EQ(Split("1,2,3,4,5", ','), (vector<string>{"1", "2", "3", "4", "5"}));
  ASSERT_EQ(Split("1", ' '), (vector<string>{"1"}));
  ASSERT_EQ(Split(" 1 ", ' '), (vector<string>{"1"}));
}

TEST(StringUtils, ReplaceAll) {
  ASSERT_EQ(ReplaceAll("1223", "12", "1"), "13");
  ASSERT_EQ(ReplaceAll("    1 2   3 ", " ", ""), "123");
  ASSERT_EQ(ReplaceAll("1:2:3", ":", "->"), "1->2->3");

  string s(100000, ' ');
  ASSERT_EQ(ReplaceAll(s, " ", ""), "");
  s = "1" + string(100, ' ');
  ASSERT_EQ(ReplaceAll(s, " ", ""), "1");
}
