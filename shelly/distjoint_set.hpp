#ifndef SHELLY_DISTJOINT_SET_HPP
#define SHELLY_DISTJOINT_SET_HPP

#include <cassert>
#include <unordered_map>
#include <vector>

namespace shelly {
inline namespace v1 {

class DistjointSet {
 public:
  DistjointSet(int n) : _n(n) {}

  void Merge(int a, int b) {
    assert(a >= 0 && a < _n);
    assert(b >= 0 && b < _n);

    int leader_a, leader_b;
    auto ita = LeaderNode(a, &leader_a);
    auto itb = LeaderNode(b, &leader_b);
    if (-ita->second < -itb->second) {
      std::swap(ita, itb);
      std::swap(leader_a, leader_b);
      std::swap(a, b);
    }
    ita->second += itb->second;
    itb->second = leader_a;
  }

  int Leader(int a) {
    assert(a >= 0 && a < _n);
    int leader;
    LeaderNode(a, &leader);
    return leader;
  }

  int GroupSize(int a) {
    assert(a >= 0 && a < _n);
    int unused;
    auto it = LeaderNode(a, &unused);
    return -it->second;
  }

 private:
  std::unordered_map<int, int>::iterator LeaderNode(int a, int* leader) {
    auto p = _node2leader.insert(std::make_pair(a, -1));
    auto it = std::move(p.first);
    std::vector<int> to_update;
    *leader = a;
    for (;;) {
      if (it->second < 0) {
        break;
      }
      to_update.push_back(*leader);
      *leader = it->second;
      it = _node2leader.find(it->second);
    }
    for (int n : to_update) {
      _node2leader[n] = *leader;
    }
    return it;
  }

  int _n;
  // leader or -group_size
  std::unordered_map<int, int> _node2leader;
};
}
}
#endif
