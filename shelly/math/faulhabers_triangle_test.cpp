#include <gtest/gtest.h>

#include "shelly/math/faulhabers_triangle.hpp"
#include "shelly/math/fraction.hpp"
#include "shelly/math/mod_int.hpp"
#include "shelly/math/math.hpp"

using namespace std;
using namespace shelly;

TEST(FaulhabersTriangle, Values) {
  auto FT = ComputeFaulhabersTriangle<Fraction<int64_t>>(10);

  int row = 5;
  vector<int64_t> numerators  {0, -1, 0, 5,  1, 1};
  vector<int64_t> denominators{1, 12, 1, 12, 2, 6};

  for (int i = 0; i < row + 1; i++) {
    ASSERT_EQ(numerators[i], FT[row][i].n);
    ASSERT_EQ(denominators[i], FT[row][i].d);
  }
}

// sum(i^100) i=1..100000
TEST(FaulhabersTriangle, SumOfPowers) {
  const int64_t kMod = 1000000007;
  typedef Fraction<ModInt<int64_t, kMod>> Fr;
  int power = 100;
  const int N = 100000;

  auto FT = ComputeFaulhabersTriangle<Fr>(power + 1);
  Fr res;

  ModInt<int64_t, kMod> t(N);
  for (int64_t i = 0; i < power + 1; i++, t *= N) {
    res += FT[power][i] * t;
  }

  int64_t mm = ModularMultiplicativeInverse(res.d.get(), kMod);
  int64_t result = (res.n * mm).get();

  ASSERT_EQ(175618146, result);
}
