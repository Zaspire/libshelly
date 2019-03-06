#ifndef _LIBSHELLY_BIG_INT_HPP_HJBNBCNBFJBEI
#define _LIBSHELLY_BIG_INT_HPP_HJBNBCNBFJBEI

#include "shelly/common.hpp"

namespace shelly {

template<class Compare>
int IntBinarySearch(int low, int hi, Compare comp) {
  if (hi - low <= 3) {
    int i = low;
    for (; i <= hi; i++) {
      if (comp(i))
        break;
    }
    assert(i != hi + 1);
    return i;
  }
  int m = low + (hi - low) / 2;
  if (comp(m)) {
    return IntBinarySearch(low, m, comp);
  } else {
    return IntBinarySearch(m, hi, comp);
  }
}

class BigInt {
public:
  BigInt(BigInt&&) = default;
  BigInt(const BigInt&) = default;
  BigInt& operator=(const BigInt&) = default;
  BigInt& operator=(BigInt&&) = default;
  BigInt(): BigInt(0) {}
  BigInt(int64_t d) {
    _negative = d < 0;
    d = std::abs(d);
    while (d) {
      _data.push_back(d % kBase);
      d /= kBase;
    }
  }
  BigInt(const std::string &a) {
    if (a.empty()) {
      _negative = false;
      return;
    }
    _negative = a[0] == '-';
    for (int i = a.size() - 1; i >= 0; i -= kT) {
      int64_t r = 0, m = 1;
      for (int k = i, j = kT; j > 0 && k >= 0; j--, k--,m*=10) {
        assert(a[k] != '-' || k == 0);
        if (a[k] == '-') {
          continue;
        }
        assert(a[k] >= '0' && a[k] <= '9');
        r = r + (a[k] - '0') * m;
      }
      _data.push_back(r);
    }
  }

  static const int32_t kBase = 1000000000; // Power of 10
  static const int kT = 9; // log10(kBase);

  BigInt& operator/=(BigInt rhs) {
    assert(rhs._data.size() && rhs._data.back());
    if (AbsComparison(*this, rhs) < 0) {
      Clear_();
      return *this;
    }
    bool sign = rhs._negative ^ _negative;

    rhs._negative = false;

    BigInt res = 0, r = 0;
    for (int i = int(_data.size()) - 1; i >= 0; i--) {
      assert(r._data.empty() || r._data.back());
      r._data.insert(r._data.begin(), _data[i]);

      if (AbsComparison(r, rhs) < 0) {
        continue;
      }

      int k = IntBinarySearch(1, kBase, [&rhs, &r](int k) {
        return rhs * k > r;
      });
      k--;

      r -= rhs * k;
      assert(r >= 0);

      res._data.insert(res._data.begin(), k);
    }

    *this = std::move(res);
    _negative = sign;
    return *this;
  }

  const static int kKaratsubeThreshold = 20;
  BigInt& KaratsubaMultiplication(const BigInt &rhs) {
    const int min_size = std::min(_data.size(), rhs._data.size());
    assert(min_size > kKaratsubeThreshold);

    const int m = min_size / 2;
    bool sign = _negative ^ rhs._negative;

    BigInt x0(_data.begin(), _data.begin() + m);
    BigInt x1(_data.begin() + m, _data.end());
    BigInt y0(rhs._data.begin(), rhs._data.begin() + m);
    BigInt y1(rhs._data.begin() + m, rhs._data.end());

    BigInt z0(x0 * y0), z2(x1 * y1);
    BigInt z1((x1 + x0) * (y1 + y0) - z2 - z0);

    *this = std::move(z0);

    AddSameSign_(z2, 2 * m);
    AddSameSign_(z1, m);

    _negative = sign;
    return *this;
  }

  BigInt& operator*=(const BigInt &rhs) {
    if (rhs._data.empty() || _data.empty()) {
      *this = 0;
      return *this;
    }

    const int min_size = std::min(_data.size(), rhs._data.size());
    if (min_size > kKaratsubeThreshold) {
      return KaratsubaMultiplication(rhs);
    }

    bool sign = _negative ^ rhs._negative;
    BigInt orig(std::move(*this));
    orig._negative = false;

    Clear_();
    _data.reserve(rhs._data.size() + orig._data.size() + 1);
    for (size_t i = 0; i < rhs._data.size(); i++) {
      BigInt t = orig;
      t *= rhs._data[i];

      AddSameSign_(t, i);
    }
    _negative = sign;
    return *this;
  }

  BigInt& operator*=(int32_t rhs) {
    if (rhs == 0 || _data.empty()) {
      Clear_();
      return *this;
    }
    if (rhs < 0) {
      rhs *= -1;
      _negative = !_negative;
    }
    int64_t r = 0;
    for (size_t i = 0; i < _data.size(); i++) {
      const int64_t t = int64_t(_data[i]) * rhs + r;
      _data[i] = t % kBase;
      r = t / kBase;
    }
    while (r) {
      _data.push_back(r % kBase);
      r /= kBase;
    }
    return *this;
  }

  BigInt& operator+=(const BigInt& rhs) {
    bool same = rhs._negative == _negative;
    if (same) { // Abs sum
      AddSameSign_(rhs, 0);
    } else {
      const BigInt *s = &rhs;
      BigInt tt;

      if (AbsComparison(*this, rhs) < 0) {
        tt = std::move(*this);
        *this = rhs;
        s = &tt;
      }

      int64_t r = 0;
      for (size_t i = 0; i < _data.size(); i++) {
        int64_t t = _data[i] - r;
        if (s->_data.size() > i) {
          t -= s->_data[i];
        }
        if (t < 0) {
          t += kBase;
          r = 1;
        } else {
          r = 0;
        }
        assert(t % kBase == t);
        _data[i] = t;
      }

      while (_data.size() && _data.back() == 0)
        _data.pop_back();
      if (_data.empty())
        this->_negative = false;
    }
    return *this;
  }
  BigInt& operator-=(const BigInt &rhs) {
    _negative = !_negative;
    *this += rhs;
    if (_data.size() > 0)
      _negative = !_negative;

    return *this;
  }
  friend int64_t operator%(const BigInt &lhs, int64_t m) {
    assert(!lhs._negative && m > 0);
    int64_t r = 0;
    const int64_t b = kBase % m;
    int64_t bi = 1;
    for (size_t i = 0; i < lhs._data.size(); i++, bi = (bi * b) % m) {
      r = (r + lhs._data[i] * bi) % m;
    }
    return r;
  }
  friend BigInt operator+(BigInt lhs, const BigInt& rhs) {
    return lhs += rhs;
  }
  friend BigInt operator*(BigInt lhs, const BigInt& rhs) {
    return lhs *= rhs;
  }
  friend BigInt operator/(BigInt lhs, const BigInt& rhs) {
    return lhs /= rhs;
  }
  friend BigInt operator-(BigInt lhs, const BigInt& rhs) {
    return lhs -= rhs;
  }
  friend bool operator!=(const BigInt &lhs, const BigInt& rhs) {
    return !(lhs == rhs);
  }
  friend bool operator==(const BigInt &lhs, const BigInt& rhs) {
    if (lhs._negative != rhs._negative)
      return false;
    if (lhs._data.size() != rhs._data.size())
      return false;
    for (size_t i = 0; i < lhs._data.size(); i++) {
      if (lhs._data[i] != rhs._data[i])
        return false;
    }
    return true;
  }
  friend bool operator<(const BigInt &lhs, const BigInt& rhs) {
    if (lhs._negative && !rhs._negative)
      return true;
    if (!lhs._negative && rhs._negative)
      return false;
    assert(lhs._negative == rhs._negative);
    int r = AbsComparison(lhs, rhs);
    if (r == 1) {
      return lhs._negative;
    }
    if (r == -1) {
      return !lhs._negative;
    }
    return false;
  }
  friend bool operator<=(const BigInt &lhs, const BigInt& rhs) {
    if (lhs._negative && !rhs._negative)
      return true;
    if (!lhs._negative && rhs._negative)
      return false;
    assert(lhs._negative == rhs._negative);
    int r = AbsComparison(lhs, rhs);
    if (r == 1) {
      return lhs._negative;
    }
    if (r == -1) {
      return !lhs._negative;
    }
    return true;
  }
  friend bool operator>(const BigInt &lhs, const BigInt& rhs) {
    return !(lhs <= rhs);
  }
  friend bool operator>=(const BigInt &lhs, const BigInt& rhs) {
    return !(lhs < rhs);
  }
private:
  int DecimalSize_() const {
    if (_data.empty()) {
      return 1;
    }
    int first = _data.back();
    assert(first);
    int res = int(_data.size() - 1) * kT;
    while (first) {
      res++;
      first /= 10;
    }
    return res;
  }
  void AddSameSign_(const BigInt& rhs, int rhs_shift) {
    assert(rhs._negative == _negative);

    _data.resize(std::max(_data.size(), rhs._data.size() + rhs_shift), 0);
    int64_t r = 0;
    for (int i = 0; i < int(_data.size()); i++) {
      int rhs_i = i - rhs_shift;
      if (rhs_i < 0) {
        continue;
      }

      int64_t s = _data[i] + r;
      if (rhs_i < int(rhs._data.size())) {
        s += rhs._data[rhs_i];
      } else if (r == 0) {
        break;
      }
      _data[i] = s % kBase;
      r = s / kBase;
    }
    if (r)
      _data.push_back(r);
  }

  // 1 lhs is bigger, -1 rhs is bigger
  static int AbsComparison(const BigInt &lhs, const BigInt& rhs) {
    assert(lhs._data.empty() || lhs._data.back());
    assert(rhs._data.empty() || rhs._data.back());
    if (lhs._data.size() > rhs._data.size())
      return 1;
    if (lhs._data.size() < rhs._data.size())
      return -1;
    if (lhs._data.empty())
      return 0;
    for (int i = int(lhs._data.size() - 1); i >= 0; i--) {
      if (lhs._data[i] > rhs._data[i])
        return 1;
      if (lhs._data[i] < rhs._data[i])
        return -1;
    }
    return 0;
  }
  BigInt(std::vector<int32_t>::const_iterator start,
         std::vector<int32_t>::const_iterator end):
    _negative(false), _data(start, end) {
    while (_data.size() && !_data.back()) {
      _data.pop_back();
    }
  }
  void Clear_() {
    _data.clear();
    _negative = false;
  }
  bool _negative;
  std::vector<int32_t> _data;

  friend std::string to_string(const BigInt &n);
};

std::string to_string(const BigInt &n) {
  std::string r;
  r.reserve(n._data.size() * BigInt::kT + 1);
  for (size_t i = 0; i < n._data.size(); i++) {
    while (i * BigInt::kT > r.size())
      r.push_back('0');
    std::string t = std::to_string(n._data[i]);
    std::reverse(t.begin(), t.end());
    r.append(t);
  }
  if (n._negative)
    r.push_back('-');
  std::reverse(r.begin(), r.end());

  if (r.empty())
    return "0";
  return r;
}

}

#endif
