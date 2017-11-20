#ifndef SHELLY_MATH_GEOMETRY_HPP_DSAFXVZGTEGDVB
#define SHELLY_MATH_GEOMETRY_HPP_DSAFXVZGTEGDVB

namespace shelly {
inline namespace v1 {

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

}
}

#endif
