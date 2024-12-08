#ifndef _SHELLY_MATH_MATH_H_FAFLHFJNBVZ843240
#define _SHELLY_MATH_MATH_H_FAFLHFJNBVZ843240

#include "shelly/common.hpp"

namespace shelly {
inline namespace v1 {

template<typename T, typename PowerType>
T FastPower(const T &a, PowerType power) {
  if (power == 0)
    return 1;
  if (power == 1)
    return a;
  T r = FastPower(a, power / 2);
  r = r * r;
  if (power % 2)
    r = r * a;
  return r;
}

std::map<int64_t, int> Factor(int64_t N) {
  std::map<int64_t, int> res;

  while (N % 2 == 0) {
    res[2]++;
    N /= 2;
  }

  for (int64_t i = 3, t = sqrt(N) + 1; i < t; i += 2) {
    int count = 0;
    while (N % i == 0) {
      N /= i;
      count++;
    }
    if (count) {
      res[i] = count;
      t = sqrt(N) + 1;
    }
  }
  if (N != 1) {
    res[N]++;
  }

  return res;
}

template<typename A>
A ModularMultiplicativeInverse(A a, A b) { // b - Modular
  std::pair<A, A> s = std::make_pair(0/*s*/, 1/*old_s*/), t = std::make_pair(1/*t*/, 0/*old_t*/);
  std::pair<A, A> r = std::make_pair(b, a);
  while (r.first != 0) {
    A quotient = r.second / r.first;
    r = std::make_pair(r.second - quotient * r.first, r.first);
    s = std::make_pair(s.second - quotient * s.first, s.first);
    t = std::make_pair(t.second - quotient * t.first, t.first);
  }
  if (s.second >= 0)
    return s.second;
  return b + s.second;
}

template<typename A>
A GreatestCommonDivisor(const A a, const A b) {
  if (a > b)
    return GreatestCommonDivisor(b, a);
  if (b % a == 0)
    return a;
  return GreatestCommonDivisor(a, b % a);
}

}
}

#endif
