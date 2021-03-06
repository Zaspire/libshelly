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

  const static int32_t kBase = 1000000000; // Power of 10
  const static int kT = 9; // log10(kBase);

  BigInt& operator/=(BigInt rhs) {
    assert(rhs._data.size() && rhs._data.back());
    if (AbsComparison(*this, rhs) < 0) {
      *this = 0;
      return *this;
    }
    bool sign = rhs._negative ^ _negative;

    rhs._negative = false;

    BigInt res = 0, r = 0;
    for (int i = int(_data.size()) - 1; i >= 0; i--) {
      r *= kBase;
      r += _data[i];
      if (r < rhs) {
        continue;
      }
      int k = IntBinarySearch(1, kBase, [&rhs, &r](int k) {
        return rhs * k > r;
      });
      k--;

      r -= rhs * k;
      assert(r >= 0);

      res *= kBase;
      res += k;
    }

    *this = std::move(res);
    _negative = sign;
    return *this;
  }

  BigInt& operator*=(const BigInt &rhs) {
    if (rhs._data.empty() || _data.empty()) {
      *this = 0;
      return *this;
    }
    bool sign = _negative ^ rhs._negative;
    BigInt orig = std::move(*this);
    orig._negative = false;
    *this = 0;
    std::vector<uint32_t> oo;
    for (size_t i = 0; i < rhs._data.size(); i++) {
      BigInt t = orig;
      t *= int32_t(rhs._data[i]);
      t._data.insert(t._data.begin(), oo.begin(), oo.end());

      *this += t;

      oo.push_back(0);
    }
    _negative = sign;
    return *this;
  }

  BigInt& operator*=(const int32_t rhs) {
    if (rhs == 0 || _data.empty()) {
      *this = 0;
      _negative = false;
      return *this;
    }
    if (rhs < 0) {
      _negative = !_negative;
    }
    int64_t r = 0;
    for (size_t i = 0; i < _data.size(); i++) {
      int64_t t = int64_t(_data[i]) * rhs + r;
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
      _data.resize(std::max(_data.size(), rhs._data.size()), 0);
      uint32_t r = 0;
      for (size_t i = 0; i < _data.size(); i++) {
        uint32_t s = _data[i] + r;
        if (rhs._data.size() > i)
          s += rhs._data[i];
        _data[i] = s % kBase;
        r = s / kBase;
      }
      if (r)
        _data.push_back(r);
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
  BigInt& operator-=(BigInt rhs) {
    //FIXME: avoid copy
    rhs._negative = !rhs._negative;
    (*this) += rhs;
    return *this;
  }
  friend int64_t operator%(const BigInt &lhs, int64_t m) {
    assert(!lhs._negative && m > 0);
    int64_t r = 0;
    static int64_t b = kBase % m;
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
  int16_t _negative;
  std::vector<uint32_t> _data;

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
