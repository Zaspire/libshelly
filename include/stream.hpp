#ifndef SHELLY_STREAM_HPP
#define SHELLY_STREAM_HPP

#include <set>
#include <vector>
#include <functional>

namespace shelly {
namespace stream {
inline namespace v1 {

template<typename Stream, typename In, typename Out, typename Func>
class MapStream;
template<typename Stream, typename In, typename Func>
class FilterStream;
template<typename Stream, typename In, typename Container>
class DistinctStream;

template<typename In, typename Out>
class BaseStream {
public:
  virtual std::pair<Out, bool> GetNext() = 0;

  std::vector<Out> ToVector() {
    std::vector<Out> res;

    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      res.push_back(p1.first);
    }

    return res;
  }

  template<typename Func>
  Out Reduce(Out Identity, Func f) {
    Out res = Identity;
    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      res = f(res, p1.first);
    }
    return res;
  }

  template<typename Func>
  auto Map(Func f) -> MapStream<BaseStream<In, Out>, In, decltype(f(In())), Func> {
    return MapStream<BaseStream<In, Out>, In,
                     decltype(f(In())), Func>(this, f);
  }

  template<typename Func>
  FilterStream<BaseStream<In, Out>, In, Func> Filter(Func f) {
    return FilterStream<BaseStream<In, Out>, In, Func>(this, f);
  }

  template<typename Container=std::set<In>>
  DistinctStream<BaseStream<In, Out>, In, Container> Distinct() {
    return DistinctStream<BaseStream<In, Out>, In, Container>(this);
  }

  template<typename Func>
  void ForEach(Func f) {
    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      f(p1.first);
    }
  }

  template<typename Func>
  bool AnyMatch(Func f) {
    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      if (f(p1.first)) {
        return true;
      }
    }
    return false;
  }

  template<typename Func>
  bool NoneMatch(Func f) {
    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      if (f(p1.first)) {
        return false;
      }
    }
    return true;
  }

  template<typename Func>
  bool AllMatch(Func f) {
    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      if (!f(p1.first)) {
        return false;
      }
    }
    return true;
  }

  size_t Count() {
    size_t res = 0;

    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      res++;
    }

    return res;
  }
};

template<typename Stream, typename In, typename Container>
class DistinctStream: public BaseStream<In, In> {
public:
  DistinctStream(Stream *stream): _stream(stream) {
  }
  std::pair<In, bool> GetNext() override {
    while (true) {
      auto p1 = _stream->GetNext();
      if (!p1.second)
        return p1;
      if (_uniq.insert(p1.first).second)
        return p1;
    }
  }
private:
  Container _uniq;
  Stream *_stream;
};

template<typename Stream, typename In, typename Func>
class FilterStream: public BaseStream<In, In> {
public:
  FilterStream(Stream *stream, Func f): _stream(stream), _f(f) {
  }
  std::pair<In, bool> GetNext() override {
    while (true) {
      auto p1 = _stream->GetNext();
      if (!p1.second)
        return p1;
      if (_f(p1.first))
        return p1;
    }
  }
private:
  Stream *_stream;
  Func _f;
};

template<typename Stream, typename In, typename Out, typename Func>
class MapStream: public BaseStream<In, Out> {
public:
  MapStream(Stream *stream, Func f): _stream(stream), _f(f) {
  }
  std::pair<Out, bool> GetNext() override {
    auto p1 = _stream->GetNext();
    if (!p1.second)
      return std::make_pair(Out(), false);
    return std::make_pair(_f(p1.first), true);
  }

private:
  Stream *_stream;
  Func _f;
};

template<typename Iterator>
class RangeStream: public BaseStream<Iterator, Iterator> {
public:
  RangeStream(Iterator it1, Iterator it2): _it1(it1), _it2(it2) {
  }

  std::pair<Iterator, bool> GetNext() override {
    if (_it1 == _it2)
      return std::make_pair(Iterator(), false);
    return std::make_pair(_it1++, true);
  }
private:
  Iterator _it1, _it2;
};

template<typename Container>
class ContainerStream: public BaseStream<typename Container::value_type, typename Container::value_type> {
public:
  ContainerStream(const Container &c): _it(c.cbegin()), _end(c.cend()) {
  }
  std::pair<typename Container::value_type, bool> GetNext() override {
    if (_it == _end)
      return std::make_pair(typename Container::value_type(), false);
    return std::make_pair(*(_it++), true);
  }
private:
  typename Container::const_iterator _it, _end;
};

template<typename T>
RangeStream<T> Range(T a, T b) {
  return RangeStream<T>(a, b);
}

template<typename Container>
ContainerStream<Container> From(const Container &c) {
  return ContainerStream<Container>(c);
}

}
}
}

#endif
