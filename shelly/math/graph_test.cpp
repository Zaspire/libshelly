#include "shelly/math/graph.hpp"

#include <gtest/gtest.h>

namespace shelly {

TEST(Graph, Dijkstra) {
  AdjacencyListGraph g(6);
  g.AddEdges({Graph::Edge(0, 1, 7),
      Graph::Edge(0, 2, 9),
      Graph::Edge(0, 5, 14),
      Graph::Edge(1, 2, 10),
      Graph::Edge(1, 3, 15),
      Graph::Edge(2, 3, 11),
      Graph::Edge(2, 5, 2),
      Graph::Edge(3, 4, 6),
      Graph::Edge(4, 5, 9)});

  std::vector<int> distances = DijkstraShortestPath(g, 0);
  EXPECT_EQ(distances, (std::vector<int>{0, 7, 9, 20, 20, 11}));
}

}  // namespace shelly
