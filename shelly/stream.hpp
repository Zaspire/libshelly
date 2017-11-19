#ifndef SHELLY_STREAM_HPP
#define SHELLY_STREAM_HPP

#include <set>
#include <vector>
#include <cassert>
#include <algorithm>
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
template<typename Stream, typename In, typename Comp>
class SortedStream;
template<typename Stream1, typename Stream2>
class ZipStream;

template<typename Out>
class BaseStream {
public:
  virtual std::pair<Out, bool> GetNext() = 0;
  virtual BaseStream<Out>& Limit(size_t l) = 0;
  virtual BaseStream<Out>& Skip(size_t l) = 0;
  virtual ~BaseStream<Out>() = default;

  typedef Out OutType;

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
  auto Map(Func f) -> MapStream<BaseStream<Out>, Out, decltype(f(Out())), Func> {
    return MapStream<BaseStream<Out>, Out,
                     decltype(f(Out())), Func>(this, f);
  }

  template<typename Func>
  FilterStream<BaseStream<Out>, Out, Func> Filter(Func f) {
    return FilterStream<BaseStream<Out>, Out, Func>(this, f);
  }

  template<typename Container=std::set<Out>>
  DistinctStream<BaseStream<Out>, Out, Container> Distinct() {
    return DistinctStream<BaseStream<Out>, Out, Container>(this);
  }

  template<typename Comp = std::less<Out>>
  SortedStream<BaseStream<Out>, Out, Comp> Sorted(Comp c = std::less<Out>()) {
    return SortedStream<BaseStream<Out>, Out, Comp>(this, c);
  }

  template<typename Stream2>
  ZipStream<BaseStream<Out>, Stream2> Zip(const Stream2 &other) {
    // all temporaries are destroyed as the last step in
    // evaluating the full-expression that (lexically) contains the point where they were created
    return ZipStream<BaseStream<Out>, Stream2>(this, &const_cast<Stream2&>(other));
  }

  template<typename S = Out>
  S Sum(S o = 0) {
    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      o += p1.first;
    }
    return o;
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

  std::pair<Out, bool> FindAny() {
    return GetNext();
  }

  template<typename Comparator = std::less<Out>>
  std::pair<Out, bool> Min(Comparator c = std::less<Out>()) {
    std::pair<Out, bool> m;
    m.second = false;
    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      if (!m.second || c(p1.first, m.first))
        m = p1;
    }
    return m;
  }

  template<typename Comparator = std::less<Out>>
  std::pair<Out, bool> Max(Comparator c = std::less<Out>()) {
    std::pair<Out, bool> m;
    m.second = false;
    while (true) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
      if (!m.second || c(m.first, p1.first))
        m = p1;
    }
    return m;
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
class DistinctStream: public BaseStream<In> {
public:
  DistinctStream(Stream *stream): _stream(stream), _limit(-1) {
  }

  std::pair<In, bool> GetNext() override {
    while (true) {
      std::pair<In, bool> p1;
      if (_limit <= _uniq.size()) {
        p1.second = false;
        return p1;
      }
      p1 = _stream->GetNext();
      if (!p1.second)
        return p1;
      if (_uniq.insert(p1.first).second)
        return p1;
    }
  }

  BaseStream<In>& Skip(size_t l) override {
    while (l--) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
    }
    return *this;
  }

  BaseStream<In>& Limit(size_t l) override {
    _limit = l + _uniq.size();
    return *this;
  }

private:
  Container _uniq;
  Stream *_stream;
  size_t _limit;
};

template<typename Stream, typename In, typename Comp>
class SortedStream: public BaseStream<In> {
public:
  SortedStream(Stream *stream, Comp c): _stream(stream) {
    while (true) {
      auto p1 = _stream->GetNext();
      if (!p1.second)
        break;
      _all.push_back(p1.first);
    }
    std::sort(_all.begin(), _all.end(), c);
    _it = _all.begin();
  }

  BaseStream<In>& Skip(size_t l) override  {
    _it += l;
    return *this;
  }

  BaseStream<In>& Limit(size_t l) override {
    size_t offset = _it - _all.begin();
    l += offset;
    if (l < _all.size())
      _all.resize(l);
    return *this;
  }

  std::pair<In, bool> GetNext() override {
    std::pair<In, bool> res;
    if (_it == _all.end()) {
      res.second = false;
      return res;
    }
    res.second = true;
    res.first = *_it;

    _it++;
    return res;
  }
private:
  typename std::vector<In>::const_iterator _it;
  std::vector<In> _all;
  Stream *_stream;
};

template<typename Stream, typename In, typename Func>
class FilterStream: public BaseStream<In> {
public:
  FilterStream(Stream *stream, Func f): _stream(stream), _f(f), _limit(-1), _pos(0) {
  }

  BaseStream<In>& Skip(size_t l) override  {
    while (l--) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
    }
    return *this;
  }

  BaseStream<In>& Limit(size_t l) override {
    _limit = l + _pos;
    return *this;
  }

  std::pair<In, bool> GetNext() override {
    if (_limit <= _pos)
      return std::make_pair(In(), false);
    while (true) {
      auto p1 = _stream->GetNext();
      if (!p1.second)
        return p1;
      if (_f(p1.first)) {
        _pos++;
        return p1;
      }
    }
  }
private:
  Stream *_stream;
  Func _f;
  size_t _limit, _pos;
};

template<typename Stream, typename In, typename Out, typename Func>
class MapStream: public BaseStream<Out> {
public:
  MapStream(Stream *stream, Func f): _stream(stream), _f(f) {
  }

  BaseStream<Out>& Skip(size_t l) override  {
    _stream->Skip(l);
    return *this;
  }

  BaseStream<Out>& Limit(size_t l) override {
    _stream->Limit(l);
    return *this;
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
class RangeStream: public BaseStream<Iterator> {
public:
  RangeStream(Iterator it1, Iterator it2): _it1(it1), _it2(it2),
                                           _limit(-1), _pos(0) {
  }

  BaseStream<Iterator>& Skip(size_t l) override  {
    _it1 += l;
    _pos += l;
    return *this;
  }

  BaseStream<Iterator>& Limit(size_t l) override {
    _limit = l + _pos;
    return *this;
  }

  std::pair<Iterator, bool> GetNext() override {
    if (_it1 >= _it2 || _limit <= _pos)
      return std::make_pair(Iterator(), false);
    _pos++;
    return std::make_pair(_it1++, true);
  }
private:
  Iterator _it1, _it2;
  size_t _limit, _pos;
};

template<typename Container>
class ContainerStream: public BaseStream<typename Container::value_type> {
public:
  ContainerStream(const Container &c): _it(c.cbegin()), _end(c.cend()),
                                       _limit(-1), _pos(0) {
  }

  BaseStream<typename Container::value_type>& Skip(size_t l) override  {
    _pos += l;
    while (l--) {
      _it++;
    }
    return *this;
  }

  BaseStream<typename Container::value_type>& Limit(size_t l) override {
    _limit = l + _pos;
    return *this;
  }

  std::pair<typename Container::value_type, bool> GetNext() override {
    if (_it == _end || _limit <= _pos)
      return std::make_pair(typename Container::value_type(), false);
    _pos++;
    return std::make_pair(*(_it++), true);
  }
private:
  typename Container::const_iterator _it, _end;
  size_t _limit, _pos;
};

template<typename Container>
class CopyContainerStream: public BaseStream<typename Container::value_type> {
public:
  CopyContainerStream(const Container &c): _c(c), _delegate(_c) {
  }

  BaseStream<typename Container::value_type>& Skip(size_t l) override  {
    _delegate.Skip(l);
    return *this;
  }

  BaseStream<typename Container::value_type>& Limit(size_t l) override {
    _delegate.Limit(l);
    return *this;
  }

  std::pair<typename Container::value_type, bool> GetNext() override {
    return _delegate.GetNext();
  }
private:
  Container _c;
  ContainerStream<Container> _delegate;
};

template<typename In, typename Func>
class IterateStream: public BaseStream<In> {
public:
  IterateStream(In e, Func f):
    _element(e), _f(f), _limit(-1), _pos(0) {
  }

  BaseStream<In>& Skip(size_t l) override {
    while (l--) {
      auto p1 = GetNext();
      if (!p1.second)
        break;
    }
    return *this;
  }

  BaseStream<In>& Limit(size_t l) override {
    _limit = l + _pos;
    return *this;
  }

  std::pair<In, bool> GetNext() override {
    if (_pos >= _limit)
      return std::make_pair(_element, false);
    _pos++;
    In old = _element;
    _element = _f(_element);
    return std::make_pair(old, true);
  }
private:
  In _element;
  Func _f;
  size_t _limit, _pos;
};

template<typename Stream1, typename Stream2>
class ZipStream: public BaseStream<std::pair<typename Stream1::OutType, typename Stream2::OutType>> {
public:
  ZipStream(Stream1 *s1, Stream2 *s2): _s1(s1), _s2(s2) {
  }
  typedef std::pair<typename Stream1::OutType, typename Stream2::OutType> Out;

  BaseStream<Out>& Skip(size_t l) override {
    _s1->Skip(l);
    _s2->Skip(l);
    return *this;
  }

  BaseStream<Out>& Limit(size_t l) override {
    _s1->Limit(l);
    _s2->Limit(l);
    return *this;
  }

  std::pair<Out, bool> GetNext() override {
    std::pair<Out, bool> res;
    auto p1 = _s1->GetNext();
    auto p2 = _s2->GetNext();

    if (!p1.second || !p2.second) {
      res.second = false;
      return res;
    }
    res.second = true;
    res.first.first = p1.first;
    res.first.second = p2.first;
    return res;
  }
private:
  Stream1 *_s1;
  Stream2 *_s2;
};

template<typename T>
RangeStream<T> Range(T a, T b) {
  return RangeStream<T>(a, b);
}

template<typename Container>
ContainerStream<Container> From(const Container &c) {
  return ContainerStream<Container>(c);
}

template<typename Element, typename... Arguments>
CopyContainerStream<std::vector<Element>> Of(Element f, Arguments... args) {
  std::vector<Element> r{f, args...};
  return CopyContainerStream<std::vector<Element>>(r);
}

template<typename Element>
CopyContainerStream<std::vector<Element>> Empty() {
  std::vector<Element> r;
  return CopyContainerStream<std::vector<Element>>(r);
}

template<typename Element, typename Func>
IterateStream<Element, Func> Iterate(Element e, Func f) {
  return IterateStream<Element, Func>(e, f);
}

}
}
}

#endif
