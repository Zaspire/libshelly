#ifndef SHELLY_MATH_FAULHABERS_TRIANGLE_NJNNMNKH79878
#define SHELLY_MATH_FAULHABERS_TRIANGLE_NJNNMNKH79878

#include "shelly/math/math.hpp"
#include "shelly/math/fraction.hpp"

namespace shelly {
inline namespace v1 {

// Numbers FT[K][i] from row K are coefficients.
// sum(1^K + 2 ^ K + ... + N ^ K) = sum(FT[R][0] * N + FT[R][1] * N ^ 2 + FT[R][K] * N ^ (K + 1))
template<typename Fraction>
std::vector<std::vector<Fraction>> ComputeFaulhabersTriangle(int64_t K) {
  std::vector<std::vector<Fraction>> r(K + 1, std::vector<Fraction>(K + 1));
  r[0][0] = Fraction(1);
  for (int64_t i = 1; i <= K; i++) {
    r[i][i] = Fraction(1, i + 1);
    r[i][i - 1] = Fraction(1, 2);

    Fraction s = r[i][i] + r[i][i - 1];
    for (int64_t k = 1; k < i - 1; k++) {
      r[i][k] = r[i - 1][k - 1] * Fraction(i, k + 1);
      s += r[i][k];
    }
    if (i > 1) {
      r[i][0] = Fraction(1) - s;
    }
  }

  return r;
}

}
}

#endif
