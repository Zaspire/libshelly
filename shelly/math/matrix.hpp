#ifndef SHELLY_MATH_MATRIX_HPP_AFZVMOJOHRNO789
#define SHELLY_MATH_MATRIX_HPP_AFZVMOJOHRNO789

#include <cstddef>
#include <cassert>
#include <vector>

namespace shelly {
inline namespace v1 {

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
