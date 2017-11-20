#ifndef _SHELLY_MATH_H_FAFLHFJNBVZ843240
#define _SHELLY_MATH_H_FAFLHFJNBVZ843240

#include "shelly/common.hpp"

namespace shelly {
inline namespace v1 {

template<class A, A MOD>
class ModInt {
public:
  static const A kMod = MOD;
  A x;
  ModInt(): x(0) {}
  ModInt(A s) {
    A sigt = s % MOD;
    if(sigt < 0)
      sigt += MOD;
    x = sigt;
  }
  A get() const { return x; }

  ModInt &operator+=(ModInt o) {
    if((x += o.x) >= MOD)
      x -= MOD;
    return *this;
  }
  ModInt &operator-=(ModInt o) {
    if((x += MOD - o.x) >= MOD)
      x -= MOD;
    return *this;
  }
  //TODO: optimize
  ModInt &operator*=(ModInt o) {
    x = (uint64_t)x * o.x % MOD;
    return *this;
  }

  ModInt operator+(ModInt o) const {
    return ModInt(*this) += o;
  }
  ModInt operator-(ModInt o) const {
    return ModInt(*this) -= o;
  }
  ModInt operator*(ModInt o) const {
    return ModInt(*this) *= o;
  }
};

template<typename T>
T FastPower(const T &a, int power) {
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

std::map<long long, int> Factor(long long N) {
  std::map<long long, int> res;

  while (N % 2 == 0) {
    res[2]++;
    N /= 2;
  }

  for (long long i = 3, t = sqrt(N) + 1; i < t; i += 2) {
    while (N % i == 0) {
      N /= i;
      res[i]++;
      t = sqrt(N) + 1;
    }
  }
  if (N != 1)
    res[N]++;

  return res;
}

template<typename A>
A ModularMultiplicativeInverse(A a, A b) { // b - Modular
  std::pair<A, A> s = std::make_pair(0/*s*/, 1/*old_s*/), t = std::make_pair(1/*t*/, 0/*old_t*/);
  std::pair<A, A> r = std::make_pair(b, a);
  while (r.first != 0) {
    A quotient = r.second / r.first;
    r = make_pair(r.second - quotient * r.first, r.first);
    s = make_pair(s.second - quotient * s.first, s.first);
    t = make_pair(t.second - quotient * t.first, t.first);
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
