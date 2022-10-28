#ifndef SHELLY_MATH_FRACTION_HPP_NKLKJHB780
#define SHELLY_MATH_FRACTION_HPP_NKLKJHB780

#include <cassert>
#include <string>

#include "shelly/math/math.hpp"

namespace shelly {
inline namespace v1 {

int64_t ToInt64(int64_t n) {
  return n;
}

template<typename T>
class Fraction {
public:
  Fraction() : Fraction(0) {}
  Fraction(const T& N) {
    n = N;
    d = 1;
  }
  Fraction(const T& _n, const T& _d) {
    n = _n;
    d = _d;
    Simplify();
  }
  Fraction &operator*=(const Fraction& o) {
    n *= o.n;
    d *= o.d;

    Simplify();
    return *this;
  }
  Fraction operator*(const Fraction& o) const {
    return Fraction(*this) *= o;
  }
  Fraction &operator/=(const Fraction& o) {
    (*this) *= Fraction(o.d, o.n);
    return *this;
  }
  Fraction operator/(const Fraction& o) const {
    return Fraction(*this) /= o;
  }
  Fraction &operator+=(const Fraction& o) {
    n = n * o.d + o.n * d;
    d *= o.d;

    Simplify();
    return *this;
  }
  Fraction operator+(const Fraction& o) const {
    return Fraction(*this) += o;
  }
  void Simplify() {
    int64_t _n = ToInt64(n), _d = ToInt64(d);
    assert(_d > 0);

    if (_n == 0) {
      d = 1;
      return;
    }
    int64_t c = GreatestCommonDivisor(std::abs(_n), _d);
    n = _n / c;
    d = _d / c;
  }
  Fraction &operator-=(const Fraction& o) {
    n = n * o.d - o.n * d;
    d *= o.d;

    Simplify();
    return *this;
  }
  Fraction operator-(const Fraction& o) const {
    return Fraction(*this) -= o;
  }
  bool operator==(const Fraction& o) const {
    return n == o.n && d == o.d;
  }
  bool operator!=(const Fraction& o) const {
    return n != o.n || d != o.d;
  }
  T n, d;
};

template<typename T>
std::string to_string(const Fraction<T> &f) {
  return std::to_string(f.n) + "/" + std::to_string(f.d);
}

}
}
#endif
