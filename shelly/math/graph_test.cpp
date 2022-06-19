#include "shelly/math/graph.hpp"

#include <gtest/gtest.h>

namespace shelly {

TEST(Graph, ShortestPath) {
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

  std::vector<int> expected_shortest{0, 7, 9, 20, 20, 11};
  EXPECT_EQ(DijkstraShortestPath(g, 0), expected_shortest);
  EXPECT_EQ(RestorePath(g, 0, 4, expected_shortest),
            (std::vector<int>{0, 2, 5, 4}));

  std::vector<int> expected_bfs{0, 1, 1, 2, 2, 1};
  EXPECT_EQ(BreadthFirstSearch(g, 0, 4, ExplorationOrderRecorder),
            expected_bfs);
  EXPECT_EQ(RestorePath(g, 0, 4, expected_bfs, ExplorationOrderRecorder),
            (std::vector<int>{0, 5, 4}));
}

TEST(Graph, MaxFlow) {
  AdjacencyListGraph g(6);
  g.AddEdges({Graph::Edge(0, 1, 1, false),
      Graph::Edge(0, 2, 4, false),
      Graph::Edge(0, 4, 4, false),
      Graph::Edge(1, 5, 5, false),
      Graph::Edge(2, 1, 3, false),
      Graph::Edge(2, 3, 2, false),
      Graph::Edge(3, 5, 3, false),
      Graph::Edge(4, 3, 2, false),
      Graph::Edge(4, 5, 1, false)});
  std::vector<std::vector<int>> flows = MaxFlow(g, 0, 5);
  int target_flow = 0;
  for (int n : flows[5]) {
    target_flow += n;
  }
  int source_flow = 0;
  for (int n : flows[0]) {
    source_flow += n;
  }
  EXPECT_EQ(-target_flow, source_flow);
  EXPECT_EQ(source_flow, 8);
}


}  // namespace shelly
