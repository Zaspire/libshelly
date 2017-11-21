#ifndef SHELLY_MATH_MOD_INT_HPP_VDSVDSVX867
#define SHELLY_MATH_MOD_INT_HPP_VDSVDSVX867

namespace shelly {
inline namespace v1 {

template<class A, A MOD>
class ModInt {
public:
  static const A kMod = MOD;
  ModInt(): x(0) {}
  ModInt(A s) {
    A sigt = s % MOD;
    if(sigt < 0)
      sigt += MOD;
    x = sigt;
  }
  A get() const { return x; }

  ModInt &operator+=(const ModInt& o) {
    if((x += o.x) >= MOD)
      x -= MOD;
    return *this;
  }
  ModInt &operator-=(const ModInt& o) {
    if((x += MOD - o.x) >= MOD)
      x -= MOD;
    return *this;
  }

  ModInt &operator*=(const ModInt& o) {
    x = (uint64_t)x * o.x % MOD;
    return *this;
  }

  ModInt operator+(const ModInt& o) const {
    return ModInt(*this) += o;
  }
  ModInt operator-(const ModInt& o) const {
    return ModInt(*this) -= o;
  }
  ModInt operator*(const ModInt& o) const {
    return ModInt(*this) *= o;
  }
private:
  A x;
};

template<class A, A MOD>
int64_t ToInt64(const ModInt<A, MOD> &n) {
  return n.get();
}

template<class A, A MOD>
std::string to_string(const ModInt<A, MOD> &mi) {
  return std::to_string(mi.get());
}

}
}

#endif
