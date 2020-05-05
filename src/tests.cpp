#include <gtest/gtest.h>
#include <frsky/sport.hpp>

using namespace frsky;

TEST(SVars, FixedPoint)
{
  EXPECT_EQ(sport::FixedPoint(10u).encode(), 10 << 8);
}
