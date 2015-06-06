#ifndef _SHELLY_MATH_H_FAFLHFJNBVZ843240
#define _SHELLY_MATH_H_FAFLHFJNBVZ843240

#include "common.hpp"

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
  A ModularMultiplicativeInverse(A a, A b) {
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

  template<typename A>
  class Line {
  public:
    Line(A x1, A y1, A x2, A y2): _x1(x1), _y1(y1), _x2(x2), _y2(y2) {
      assert((x2 - x1) || (y2 - y1));
    }

    bool Check(A x, A y) {
      return (x - _x1) * (_y2 - _y1) == (y - _y1) * (_x2 - _x1);
    }
  private:
    A _x1, _x2, _y1, _y2;
  };

  template<typename A>
  class Matrix {
  public:
    Matrix(int rows, int columns): _data(rows, std::vector<A>(columns, 0)) {
      assert(rows > 0);
      assert(columns > 0);
    }
    Matrix<A> transpose() const {
      Matrix<A> r(_data[0].size(), _data.size());
      for (size_t i = 0; i < _data.size(); i++) {
        for (size_t k = 0; k < _data[0].size(); k++) {
          r._data[k][i] = _data[i][k];
        }
      }
      return r;
    }
    Matrix<A> operator*(const Matrix<A> &other) const {
      assert(_data[0].size() == other._data.size());
      Matrix<A> r(_data.size(), other._data[0].size());

      for (size_t i = 0; i < _data.size(); i++) {
        for (size_t k = 0; k < other._data[0].size(); k++) {
          A t = 0;
          for (size_t j = 0; j < _data[0].size(); j++)
            t += _data[i][j] * other._data[j][k];
          r._data[i][k] = t;
        }
      }

      return r;
    }
    Matrix<A> operator-(const Matrix<A> &other) const {
      assert(_data.size() == other._data.size());
      assert(_data[0].size() == other._data[0].size());
      Matrix<A> r(_data.size(), _data[0].size());

      for (size_t i = 0; i < _data.size(); i++) {
        for (size_t k = 0; k < _data[0].size(); k++) {
          r._data[i][k] = _data[i][k] - other._data[i][k];
        }
      }

      return r;
    }
    Matrix<A> operator+(const Matrix<A> &other) const {
      assert(_data.size() == other._data.size());
      assert(_data[0].size() == other._data[0].size());
      Matrix<A> r(_data.size(), _data[0].size());

      for (size_t i = 0; i < _data.size(); i++) {
        for (size_t k = 0; k < _data[0].size(); k++) {
          r._data[i][k] = _data[i][k] + other._data[i][k];
        }
      }

      return r;
    }
    std::vector<std::vector<A>> _data;
  };
}
}

#endif
