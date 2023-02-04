#ifndef SHELLY_MATH_GRAPH_HPP
#define SHELLY_MATH_GRAPH_HPP

#include <memory>
#include <cassert>
#include <functional>
#include <limits>
#include <queue>
#include <vector>

namespace shelly {
inline namespace v1 {

static constexpr int kUnreachableVertex = std::numeric_limits<int>::max();
static constexpr int kInfiniteFlow = std::numeric_limits<int>::max() - 1;

class Graph {
public:
  struct Edge {
    Edge(int f, int t, int w, int d = false) : from(f), to(t), weight(w), directed(d) {};
    int from, to, weight;
    bool directed = false;
  };

  virtual void AddEdge(int from, int to, int weight) = 0;
  void AddEdges(const std::vector<Edge>& edges) {
    for (const Edge &e : edges) {
      AddEdge(e.from, e.to, e.weight);
      if (!e.directed) {
        AddEdge(e.to, e.from, e.weight);
      }
    }
  }

  virtual void InsertOrUpdate(int from, int to, int diff) = 0;

  virtual int VertexCount() const = 0;
  virtual void EnumerateEdges(
      int vertex, const std::function<bool(int other_vertex, int weight)>& cb) const = 0;
  virtual void EnumerateInboundEdges(
      int vertex, const std::function<bool(int other_vertex, int weight)>& cb) const = 0;
  virtual std::unique_ptr<Graph> Clone() const = 0;
};

class AdjacencyListGraph : public Graph {
public:
  AdjacencyListGraph(int vertex_count) : outbound_(vertex_count),
                                         inbound_(vertex_count) {
  }

  void AddEdge(int from, int to, int weight) override {
    assert(!HasEdge(from, to));
    outbound_[from].emplace_back(to, weight);
    inbound_[to].emplace_back(from, weight);
  }

  void InsertOrUpdate(int from, int to, int diff) override {
    bool updated = false;

    int w;
    for (auto& p : outbound_[from]) {
      if (p.first != to) {
        continue;
      }
      updated = true;
      p.second += diff;
      w = p.second;
      break;
    }
    if (!updated) {
      AddEdge(from, to, diff);
      return;
    }
    bool found_inbound = false;
    for (auto& p : inbound_[to]) {
      if (p.first != from) {
        continue;
      }
      found_inbound = true;
      p.second += diff;
      assert(w == p.second);
      break;
    }
    assert(found_inbound);
  }

  int VertexCount() const override {
    return outbound_.size();
  }

  void EnumerateEdges(
      int vertex,
      const std::function<bool(int other_vertex, int weight)>& cb) const override {
    for (const auto& p : outbound_[vertex]) {
      int other_vertex, weight;
      std::tie(other_vertex, weight) = p;
      if (!cb(other_vertex, weight)) {
        return;
      }
    }
  }

  void EnumerateInboundEdges(
      int vertex,
      const std::function<bool(int other_vertex, int weight)>& cb) const override {
    for (const auto& p : inbound_[vertex]) {
      int other_vertex, weight;
      std::tie(other_vertex, weight) = p;
      if (!cb(other_vertex, weight)) {
        return;
      }
    }
  }

  std::unique_ptr<Graph> Clone() const override {
    return std::unique_ptr<AdjacencyListGraph>(new AdjacencyListGraph(*this));
  }

private:
  bool HasEdge(int from, int to) const {
    for (const auto& p : outbound_[from]) {
      if (p.first == to) {
        return true;
      }
    }
    return false;
  }

  // pair<vertex, distance>
  std::vector<std::vector<std::pair<int, int>>> outbound_;
  std::vector<std::vector<std::pair<int, int>>> inbound_;
};

std::vector<int> DijkstraShortestPath(const Graph& graph, int source) {
  std::vector<int> distances(graph.VertexCount(), kUnreachableVertex);
  std::vector<bool> processed(graph.VertexCount(), false);
  distances[source] = 0;

  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                      std::greater<std::pair<int, int>>> q;
  q.emplace(0, source);
  while (!q.empty()) {
    int vertex, distance;
    std::tie(distance, vertex) = q.top();
    q.pop();

    if (processed[vertex]) {
      continue;
    }
    processed[vertex] = true;

    graph.EnumerateEdges(
        vertex, [distance, &q, &processed, &distances](int other_vertex, int weight) {
      if (distance + weight >= distances[other_vertex]) {
        return true;
      }
      assert(!processed[other_vertex]);
      distances[other_vertex] = distance + weight;
      q.emplace(distance + weight, other_vertex);
      return true;
    });
  }

  return distances;
}

int ExplorationOrderRecorder(
    int from, int to,
    const std::vector<int>& explored, int weight) {
  if (from == to) {
    return 0;
  }
  return explored[from] + 1;
}

std::vector<int> BreadthFirstSearch(
    const Graph& graph, int source, int target,
    std::vector<int>& exloration_order,
    const std::function<int(int from, int to, const std::vector<int>& explored, int weight)>& node_explorer_func) {
  std::vector<int> explored(graph.VertexCount(), kUnreachableVertex);

  exloration_order = std::vector<int>(graph.VertexCount(), kUnreachableVertex);

  std::queue<int> q;

  explored[source] = node_explorer_func(source, source, explored, 0);
  exloration_order[source] = 0;
  q.push(source);

  while (!q.empty()) {
    int v = q.front();
    q.pop();
    if (v == target) {
      break;
    }

    graph.EnumerateEdges(
        v, [v, &q, &exloration_order, &explored, &node_explorer_func](int other_vertex, int weight) {
      if (explored[other_vertex] != kUnreachableVertex) {
        return true;
      }
      explored[other_vertex] = node_explorer_func(v, other_vertex, explored, weight);
      if (explored[other_vertex] != kUnreachableVertex) {
        exloration_order[other_vertex] = exloration_order[v] + 1;
        q.push(other_vertex);
      }
      return true;
    });
  }
  return explored;
}

int RestoreShortestPathFunc(
    int from, int to,
    const std::vector<int>& explored, int weight) {
  if (from == to) {
    return 0;
  }
  return explored[from] + weight;
}

std::vector<int> RestorePath(
    const Graph& graph, int source, int destination,
    const std::vector<int>& explored,
    const std::vector<int>& exploration_order = {},// if needed e.g. MaxFlow
    const std::function<int(int from, int to, const std::vector<int>& explored, int weight)>& node_explorer_func = RestoreShortestPathFunc) {
  assert(explored[destination] != kUnreachableVertex);
  std::vector<int> path;
  std::vector<bool> visited(graph.VertexCount(), false);
  for (int current_vertex = destination; current_vertex != source;) {
    path.push_back(current_vertex);

    assert(!visited[current_vertex]);
    visited[current_vertex] = true;

    int next = kUnreachableVertex;
    graph.EnumerateInboundEdges(
        current_vertex, [current_vertex, &exploration_order, &explored,
                         &next, &node_explorer_func, &visited](int other_vertex, int weight) {
      if (visited[other_vertex] || explored[other_vertex] == kUnreachableVertex) {
        return true;
      }
      if (!exploration_order.empty()
          && exploration_order[other_vertex] + 1 != exploration_order[current_vertex]) {
        return true;
      }
      int computed = node_explorer_func(other_vertex, current_vertex, explored, weight);
      if (computed == explored[current_vertex]) {
        next = other_vertex;
        return false;
      }
      return true;
    });
    assert(next != kUnreachableVertex);
    current_vertex = next;
  }
  path.push_back(source);
  return std::vector<int>(path.rbegin(), path.rend());;
}

std::vector<std::vector<int>> MaxFlow(const Graph& graph, int source, int target) {
  assert(source != target);

  std::vector<std::vector<int>> res(graph.VertexCount(),
                                    std::vector<int>(graph.VertexCount(), 0));
  std::unique_ptr<Graph> copy = graph.Clone();

  auto FlowRecorder = [](int from, int to, const std::vector<int>& explored, int weight) -> int {
    if (from == to) {
      return kInfiniteFlow;
    }
    int res = std::min(weight, explored[from]);
    if (res > 0) {
      return res;
    }
    return kUnreachableVertex;
  };
  for (;;) {
    std::vector<int> exploration_order;
    std::vector<int> explored = BreadthFirstSearch(
        *copy, source, target, exploration_order, FlowRecorder);
    if (explored[target] == kUnreachableVertex) {
      break;
    }
    int value = explored[target];
    std::vector<int> flow = RestorePath(
        *copy, source, target, explored, exploration_order, FlowRecorder);
    for (int i = 1; i < int(flow.size()); i++) {
      res[flow[i - 1]][flow[i]] += value;
      res[flow[i]][flow[i - 1]] -= value;
      copy->InsertOrUpdate(flow[i - 1], flow[i], -value);
      copy->InsertOrUpdate(flow[i], flow[i - 1], value);
    }
  }
  return res;
}

}
}

#endif
