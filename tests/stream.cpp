#include <gtest/gtest.h>

#include "stream.hpp"

using namespace std;
using namespace shelly;

TEST(Stream, From) {
  vector<int> b{1, 2, 3};
  ASSERT_EQ(stream::From(b).Count(), 3U);
  ASSERT_EQ(stream::From(map<int, int>{{1, 1}, {2, 2}}).Count(), 2U);
}

TEST(Stream, Filter) {
  size_t t1 = stream::Range(1, 101).Filter([](int a) {
    return a % 2 == 0;
  }).Map([](int a) {
    return to_string(a);
  }).Count();
  ASSERT_EQ(t1, 50U);
}

TEST(Stream, RangeStreamFromIterator) {
  vector<long> r{1, 2, 3, 4};

  stream::RangeStream<vector<long>::iterator> a(r.begin(), r.end());
  auto t1 = a.Map([](vector<long>::iterator a) {
    return *a + 5;
  }).ToVector();

  ASSERT_EQ(t1, (vector<long>{6, 7, 8, 9}));
}

TEST(Stream, AnyMatch) {
  bool b = stream::Range(1, 1000000000).AnyMatch([](int a) {
    return a % 5 == 0;
  });
  ASSERT_EQ(b, true);

  b = stream::Range(1, 50).AnyMatch([](int a) {
    return a % 50 == 0;
  });
  ASSERT_EQ(b, false);
}

TEST(Stream, NoneMatch) {
  bool b = stream::Range(1, 1000000000).NoneMatch([](int a) {
    return a % 5 == 0;
  });
  ASSERT_EQ(b, false);

  b = stream::Range(1, 50).NoneMatch([](int a) {
    return a % 50 == 0;
  });
  ASSERT_EQ(b, true);
}

TEST(Stream, AllMatch) {
  bool b = stream::Range(1, 1000000000).AllMatch([](int a) {
    return a % 5 == 0;
  });
  ASSERT_EQ(b, false);

  b = stream::Range(1, 50).AllMatch([](int a) {
    return a % 50 != 0;
  });
  ASSERT_EQ(b, true);
}

TEST(Stream, Sorted) {
  vector<int> b{45, 2, 3, 125};
  ASSERT_EQ(stream::From(b).Sorted().ToVector(), (vector<int>{2, 3, 45, 125}));

  auto t1 = stream::From(b).Sorted([](int a, int b) {
    return a > b;
  }).ToVector();
  ASSERT_EQ(t1, (vector<int>{125, 45, 3, 2}));
}

TEST(Stream, FindAny) {
  auto p1 = stream::Range(1, 1000000000).Filter([](int a) {
    return a % 5 == 0;
  }).FindAny();
  ASSERT_EQ(p1.second, true);
  ASSERT_EQ(p1.first, 5);

  p1 = stream::Range(1, 50).Filter([](int a) {
    return a % 50 == 0;
  }).FindAny();
  ASSERT_EQ(p1.second, false);
}

TEST(Stream, ToVector) {
  ASSERT_EQ((stream::Range(1, 4).ToVector()), (vector<int>{1, 2, 3}));

  auto t1 = stream::Range(1, 4).Map([](int n) {
    return to_string(n);
  }).ToVector();
  ASSERT_EQ(t1, (vector<string>{"1", "2", "3"}));

  ASSERT_EQ(stream::From(t1).ToVector(), t1);
}

TEST(Stream, Distinct) {
  ASSERT_EQ((stream::Range(1, 4).Distinct().Count()), 3U);

  auto t1 = stream::Range(1, 100).Map([](int) {
    return 1;
  }).Distinct().ToVector();
  ASSERT_EQ(t1, (vector<int>{1}));
}

TEST(Stream, Reduce) {
  int a = stream::Range(1, 6).Reduce(0, [](int a, int b) {
    return a + b;
  });
  ASSERT_EQ(a, 15);
  a = stream::Range(1, 6).Reduce(1, [](int a, int b) {
    return a * b;
  });
  ASSERT_EQ(a, 120);
  string s = stream::Range(1, 6).Map([](int a) {
    return to_string(a);
  }).Reduce("", [](string a, string b) {
    return a + b;
  });
  ASSERT_EQ(s, "12345");
}
