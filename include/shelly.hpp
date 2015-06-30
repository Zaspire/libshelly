#ifndef _SHELLY_H_FJABMC897532786
#define _SHELLY_H_FJABMC897532786

#include "common.hpp"

#include <array>
#include <sys/time.h>
#include <regex>
#include <numeric>
#include <random>

namespace shelly {

std::string to_string(const std::string &s) {
  return s;
}

template<typename A, typename B>
std::string to_string(const std::pair<A, B> &p) {
  using std::to_string;
  using shelly::to_string;

  return std::string("<") + to_string(p.first) + ", " + to_string(p.second) + ">";
}
template<typename A, typename B>
std::string to_string(const std::map<A, B> &m) {
  std::string r = "{ ";

  bool first = true;
  for (const auto &p: m) {
    if (!first)
      r += ", ";
    first = false;
    r += to_string(p);
  }
  r += "}";
  return r;
}
template<typename A>
std::string to_string(const std::vector<A> &v) {
  using std::to_string;
  using shelly::to_string;
  std::string r = "[ ";

  bool first = true;
  for (const A &e: v) {
    if (!first)
      r += ", ";
    first = false;
    r += to_string(e);
  }

  r += "]";
  return r;
}
template<typename A>
std::string to_string(const std::set<A> &v) {
  std::string r = "[ ";

  bool first = true;
  for (const A &e: v) {
    if (!first)
      r += ", ";
    first = false;
    r += std::to_string(e);
  }

  r += "]";
  return r;
}

template<typename T>
std::size_t MakeHash(const T& v) {
  return std::hash<T>()(v);
}

void HashCombine(std::size_t& h, const std::size_t& v) {
  h ^= v + 0x9e3779b9 + (h << 6) + (h >> 2);
}

template<typename T>
struct HashContainer {
  size_t operator()(const T& v) const {
    size_t h = 0;
    for (const auto& e : v) {
      HashCombine(h, MakeHash(e));
    }
    return h;
  }
};

template<class T, class Func>
T Filter(const T &container, Func f1) {
  T res;

  for (auto &e: container) {
    if (f1(e))
      res.push_back(e);
  }

  return res;
}

template<typename C, typename Func>
auto Map(const C &container, Func f1) -> std::vector<decltype(f1(*container.begin()))> {
  std::vector<decltype(f1(*container.begin()))> res;

  for (auto &e: container) {
    res.push_back(f1(e));
  }

  return res;
}

int LongestCommonSubsequence(const std::string &s1, const std::string &s2) {
  if (s1.empty() || s2.empty())
    return 0;
  std::vector<std::vector<size_t>> dp(s1.size() + 1, std::vector<size_t>(s2.size() + 1, 0));

  for (size_t i = 1; i <= s1.size(); i++) {
    for (size_t k = 1; k <= s2.size(); k++) {
      if (s1[i - 1] == s2[k - 1]) {
	dp[i][k] = dp[i - 1][k - 1] + 1;
	continue;
      }
      dp[i][k] = std::max(dp[i][k - 1], dp[i - 1][k]);
    }
  }

  assert(dp[s1.size()][s2.size()] <= s1.size() && dp[s1.size()][s2.size()] <= s2.size());

  return dp[s1.size()][s2.size()];
}

namespace internal {
  void args2vector(std::vector<std::string> &) {
  }

  template<typename T, typename... Args>
  void args2vector(std::vector<std::string> &out, T &value, Args... args) {
    using std::to_string;
    using shelly::to_string;
    out.push_back(to_string(value));
    args2vector(out, args...);
  }
}

template<typename... Args>
std::string Format(const std::string &str, Args... rest) {
  std::vector<std::string> args;

  internal::args2vector(args, rest...);

  std::string out;
  out.reserve(str.size() * 2);
  for (size_t i = 0; i < str.size(); i++) {
    if (str[i] != '%') {
      out.push_back(str[i]);
      continue;
    }
    if (i + 1 == str.size())
      throw std::logic_error("bad format string");
    if (str[i + 1] == '%') {
      out.push_back('%');
      i++;
      continue;
    }

    int k = 0;
    std::string n;
    while (str[i + k + 1] >= '0' && str[i + k + 1] <= '9') {
      n.push_back(str[i + k + 1]);
      k++;
    }

    if (n.empty())
      throw std::logic_error("bad format string");

    size_t j = std::atoi(n.c_str());
    if (j >= args.size())
      throw std::logic_error("incorrect arg number");

    out += args[j];

    i += k;
  }
  return out;
}

template<typename... T>
void Sort(std::vector<T...> &a) {
  std::sort(a.begin(), a.end());
}

class Timer final {
 public:
  Timer(const std::string &name = ""): _name(name) {
    timeval now;
    gettimeofday(&now, NULL);
    _start = timeeval2longlong(now);
  }
  ~Timer() {
    timeval now;
    gettimeofday(&now, NULL);
    if (_name.size())
      std::cout << _name << " ";
    std::cout << "Interval:" <<timeeval2longlong(now) - _start << std::endl;
  }
 private:
  std::string _name;
  unsigned long long _start;

  unsigned long long timeeval2longlong(const timeval &time) {
    return time.tv_usec + (unsigned long long)time.tv_sec * 1000000;
  }

  DISALLOW_COPY_AND_ASSIGN(Timer);
};

}

namespace std {

template<typename... T>
struct hash<std::vector<T...>>: shelly::HashContainer<std::vector<T...>> {};

};

#endif
