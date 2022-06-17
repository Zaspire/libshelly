#ifndef SHELLY_MATH_GRAPH_HPP
#define SHELLY_MATH_GRAPH_HPP

#include <cassert>
#include <functional>
#include <limits>
#include <queue>
#include <vector>

namespace shelly {
inline namespace v1 {

static constexpr int kUnreachableVertex = std::numeric_limits<int>::max();

class Graph {
public:
  struct Edge {
    Edge(int f, int t, int w) : from(f), to(t), weight(w) {};
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

  virtual int VertexCount() const = 0;
  virtual void EnumerateEdges(
      int vertex, std::function<void(int other_vertex, int weight)> cb) const = 0;
};

class AdjacencyListGraph : public Graph {
public:
  AdjacencyListGraph(int vertex_count) : adjacency_(vertex_count) {
  }

  void AddEdge(int from, int to, int weight) override {
    adjacency_[from].emplace_back(to, weight);
  }

  int VertexCount() const override {
    return adjacency_.size();
  }

  void EnumerateEdges(
      int vertex,
      std::function<void(int other_vertex, int weight)> cb) const override {
    for (const auto& p : adjacency_[vertex]) {
      int other_vertex, weight;
      std::tie(other_vertex, weight) = p;
      cb(other_vertex, weight);
    }
  }

private:
  // pair<vertex, distance>
  std::vector<std::vector<std::pair<int, int>>> adjacency_;
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
        return;
      }
      assert(!processed[other_vertex]);
      distances[other_vertex] = distance + weight;
      q.emplace(distance + weight, other_vertex);
    });
  }

  return distances;
}

}
}

#endif
