#include <gtest/gtest.h>

#include "stream.hpp"

using namespace std;
using namespace shelly;

struct A {
};

TEST(Stream, From) {
  vector<int> b{1, 2, 3};
  ASSERT_EQ(stream::From(b).Count(), 3U);
  ASSERT_EQ(stream::From(map<int, int>{{1, 1}, {2, 2}}).Count(), 2U);

  ASSERT_EQ(stream::From(vector<A>{}).Count(), 0U);
}

TEST(Stream, Filter) {
  size_t t1 = stream::Range(3, 101).Filter([](int a) {
    return a % 2 == 0;
  }).Map([](int a) {
    return to_string(a);
  }).Count();
  ASSERT_EQ(t1, 49U);
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

TEST(Stream, Zip) {
  auto a = stream::Range(10, 14);
  ASSERT_EQ(a.GetNext().first, 10);

  ASSERT_EQ(stream::Range(1, 10).Zip(stream::Range(1, 9)).Count(), 8U);
  ASSERT_EQ(stream::Range(1, 5).Zip(stream::Range(1, 9)).Count(), 4U);

  bool b = stream::Range(1, 5).Zip(stream::Range(11, 15)).AllMatch([](std::pair<int, int> a) {
    return a.first + 10 == a.second;
  });
  ASSERT_EQ(b, true);
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

  auto t1 = stream::Range(1, 100).Map([](int n) {
    return n % 3;
  }).Distinct().ToVector();
  ASSERT_EQ(t1, (vector<int>{1, 2, 0}));
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

TEST(Stream, Limit) {
  ASSERT_EQ(stream::Range(1, 6).Limit(2).Count(), 2U);
  ASSERT_EQ(stream::Range(1, 6).Limit(0).Count(), 0U);
  ASSERT_EQ(stream::From(vector<int>{1, 2, 3, 4}).Limit(2).Count(), 2U);

  ASSERT_EQ(stream::Range(1, 6).Limit(10).Count(), 5U);

  size_t t1 = stream::Range(1, 100).Filter([](int i) {
    return i % 2 == 0;
  }).Limit(3).Count();
  ASSERT_EQ(t1, 3U);

  ASSERT_EQ(stream::From(vector<int>{100, 2, 3, 1}).Sorted().Limit(3).ToVector(), (vector<int>{1, 2, 3}));

  ASSERT_EQ(stream::From(vector<int>{2, 2, 3, 1, 3, 4, 5}).Distinct().Limit(4).ToVector(), (vector<int>{2, 3, 1, 4}));
}

TEST(Stream, Of) {
  ASSERT_EQ(stream::Of(1, 2, 3).ToVector(), (vector<int>{1, 2, 3}));
  ASSERT_EQ(stream::Of(1).ToVector(), (vector<int>{1}));
}

TEST(Stream, Min) {
  ASSERT_EQ(stream::Of(5, 1, 2, 3).Min().first, 1);
  ASSERT_TRUE(stream::Of(5, 1, 2, 3).Min().second);
  ASSERT_FALSE(stream::From(vector<int>{}).Min().second);
}

TEST(Stream, Max) {
  ASSERT_EQ(stream::Of(5, 1, 2, 3).Max().first, 5);
  ASSERT_TRUE(stream::Of(5, 1, 2, 3).Max().second);
  ASSERT_FALSE(stream::From(vector<int>{}).Max().second);
}

TEST(Stream, Sum) {
  ASSERT_EQ(stream::Of(5, 1, 2, 3).Sum(), 11);
  ASSERT_EQ(stream::Of(5, 1, 2, 3).Sum(1U), 12U);

  ASSERT_EQ(stream::Of(5.5, 1.5, 2.5, 3.5).Sum(0U), 11U);
}

TEST(Stream, Iterate) {
  int t1 = stream::Iterate(0, [](int i) {
    return i + 5;
  }).Limit(2).Sum();
  ASSERT_EQ(t1, 5);
}

TEST(Stream, Skip) {
  int t1 = stream::Iterate(5, [](int i) {
    return i + 5;
  }).Limit(2).Skip(1).Sum();
  ASSERT_EQ(t1, 10);

  t1 = stream::Iterate(5, [](int i) {
    return i + 5;
  }).Skip(1).Limit(2).Sum();
  ASSERT_EQ(t1, 25);

  ASSERT_EQ(stream::Of(5, 1, 2, 3).Distinct().Skip(1).Limit(2).Sum(), 3);
  ASSERT_EQ(stream::Of(5, 1, 2, 3).Distinct().Limit(2).Skip(1).Sum(), 1);

  ASSERT_EQ(stream::Of(5, 1, 2, 3).Sorted().Skip(1).Limit(2).Sum(), 5);
  ASSERT_EQ(stream::Of(5, 1, 2, 3).Sorted().Limit(2).Skip(1).Sum(), 2);

  t1 = stream::Of(5, 1, 2, 3).Filter([](int) {return true;}).Skip(1).Limit(2).Sum();
  ASSERT_EQ(t1, 3);
  t1 = stream::Of(5, 1, 2, 3).Filter([](int) {return true;}).Limit(2).Skip(1).Sum();
  ASSERT_EQ(t1, 1);

  ASSERT_EQ(stream::Range(1, 6).Limit(2).Skip(1).ToVector(), (vector<int>{2}));
  ASSERT_EQ(stream::Range(1, 6).Skip(1).Limit(2).ToVector(), (vector<int>{2, 3}));

  ASSERT_EQ(stream::From(vector<int>{5, 1, 2, 3}).Skip(1).Limit(2).Sum(), 3);
  ASSERT_EQ(stream::From(vector<int>{5, 1, 2, 3}).Limit(2).Skip(1).Sum(), 1);

  ASSERT_EQ(stream::Of(5, 1, 2, 3).Skip(1).Limit(2).Sum(), 3);
  ASSERT_EQ(stream::Of(5, 1, 2, 3).Limit(2).Skip(1).Sum(), 1);

  t1 = stream::Of(5, 1, 2, 3).Zip(stream::From(vector<int>{7, 1, 22, 30})).Skip(1).Limit(2).Map([](std::pair<int, int> a) {
      return a.first + a.second;
  }).Sum();
  ASSERT_EQ(t1, 26);

  t1 = stream::Of(5, 1, 2, 3).Zip(stream::From(vector<int>{7, 1, 22, 30})).Limit(2).Skip(1).Map([](std::pair<int, int> a) {
      return a.first + a.second;
  }).Sum();
  ASSERT_EQ(t1, 2);
}

TEST(Stream, Empty) {
  ASSERT_EQ(stream::Empty<int>().FindAny().second, false);
  ASSERT_EQ(stream::Empty<A>().FindAny().second, false);
}
