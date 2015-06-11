#include <gtest/gtest.h>

#include "big_int.hpp"

using namespace std;
using namespace shelly;

TEST(BigInt, ToString) {
  ASSERT_EQ(to_string(BigInt(2545235234234ll)), "2545235234234");
  ASSERT_EQ(to_string(BigInt(1000000000007ll)), "1000000000007");
  ASSERT_EQ(to_string(BigInt(-1000000000007ll)), "-1000000000007");
  ASSERT_EQ(to_string(BigInt()), "0");
}

TEST(BigInt, Addition) {
  int64_t ans = 0;
  BigInt a;
  for (int i = 1; i < 1000; i++) {
    ans += i;
    a += i;
  }
  for (int64_t i = 1000000000007ll; i < 1000000001007ll; i++) {
    ans = ans + i;
    a += i;
  }
  ASSERT_EQ(to_string(a), to_string(ans));
}

TEST(BigInt, Comparison) {
  auto l = [](int64_t i) {
    ASSERT_TRUE(BigInt(i) == i);
    ASSERT_TRUE(i == BigInt(i));

    ASSERT_FALSE(i == BigInt(i - 1));

    ASSERT_TRUE(i < BigInt(i + 2));
    ASSERT_FALSE(i < BigInt(i - 2));
    ASSERT_FALSE(i < BigInt(i));

    ASSERT_FALSE(i > BigInt(i + 2));
    ASSERT_TRUE(i > BigInt(i - 2));
    ASSERT_FALSE(i > BigInt(i));

    ASSERT_FALSE(i >= BigInt(i + 2));
    ASSERT_TRUE(i >= BigInt(i - 2));
    ASSERT_TRUE(i >= BigInt(i));

    ASSERT_TRUE(i <= BigInt(i + 2));
    ASSERT_FALSE(i <= BigInt(i - 2));
    ASSERT_TRUE(i <= BigInt(i));
  };
  for (int i = -1000; i < 1000; i++) {
    l(i);
  }
  for (int64_t i = 1000000000000ll; i < 1000000000040ll; i++) {
    l(i);
  }
  for (int64_t i = -1000000000050ll; i < -1000000000000ll; i++) {
    l(i);
  }
}

TEST(BigInt, Modular) {
  int64_t a = 1000000000000ll;
  BigInt r1(a);
  int64_t r2 = a;
  int m = 1000000007;
  for (int i = 0; i < 100; i++) {
    r1 += a;
    r1 += 1;
    r2 = (r2 + 1 + a) % m;
    ASSERT_EQ(r2, r1 % m);
  }
}

TEST(BigInt, Subtraction) {
  int64_t ans = 1233;
  BigInt a(ans);
  for (int i = 1000; i < 2000; i++) {
    ans -= i;
    a -= i;
  }
  ans = 1233;
  a = BigInt(ans);
  for (int i = 1000; i < 2000; i++) {
    ans = ans - i;
    a -= i;
  }

  ASSERT_EQ(to_string(a), to_string(ans));
}

TEST(BigInt, Multiplication) {
  int64_t a = 1;
  BigInt b(a);
  for (int i = 1; i < 50; i++) {
    if (i % 2) {
      a *= 2;
      a -= 1;
      b *= 2;
      b -= 1;
    } else {
      a *= 2;
      a += 1;
      b *= 2;
      b += 1;
    }
  }
  ASSERT_EQ(to_string(a), to_string(b));

  a = 999999999;
  b = a;
  b *= a;
  a *= a;
  ASSERT_EQ(a, b);

  a = 1;
  b = a;
  int m = 1000000007;
  for (int i = 0; i < 200; i++) {
    a = (a * 99012) % m;
    b *= 99012;
  }
  ASSERT_EQ(b % m, a);

  int64_t f = 2, s = 3;
  BigInt bf(f), bs (s);
  for (int i = 0; i < 30; i++) {
    int64_t n;
    BigInt t = bf;
    if (i % 2) {
      n = (f * s) % m;
      bf *= bs;
    } else {
      n = (f + s) % m;
      bf += bs;
    }
    bs = t;
    s = f;
    f = n;
  }
  ASSERT_EQ(bf % m, f);
}

TEST(BigInt, Div) {
  vector<BigInt> all;
  BigInt a = 1;
  for (int i = 0; i < 100; i++) {
    a = a * -555 + 1;
    all.push_back(a);
  }
  for (int i = int(all.size()) - 1; i >= 0; i--) {
    ASSERT_EQ(to_string(all[i]), to_string(a));
    a -= 1;
    a /= -555;
  }
}
