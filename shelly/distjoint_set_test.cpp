#include "distjoint_set.hpp"

#include <gtest/gtest.h>

using namespace shelly;

TEST(DistjointSetTest, General) {
  DistjointSet ds(11);

  ds.Merge(0, 5);
  ds.Merge(1, 5);
  ds.Merge(1, 3);
  ds.Merge(0, 2);

  ds.Merge(10, 9);

  ASSERT_EQ(ds.Leader(8), 8);
  ASSERT_EQ(ds.GroupSize(ds.Leader(8)), 1);

  ASSERT_EQ(ds.Leader(2), ds.Leader(3));
  ASSERT_EQ(ds.GroupSize(ds.Leader(2)), 5);

  ASSERT_EQ(ds.GroupSize(ds.Leader(10)), 2);
}
