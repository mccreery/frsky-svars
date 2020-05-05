#include <gtest/gtest.h>
#include <frsky/sport.hpp>

using namespace frsky;

TEST(FixedPoint, Signed) {
    EXPECT_EQ(sport::FixedPoint(50).encode(), 50 << 8);
    EXPECT_EQ(sport::FixedPoint(-50).encode(), (signed)0xffffce00);
}

TEST(FixedPoint, Unsigned) {
    EXPECT_EQ(sport::FixedPoint(10u).encode(), 10 << 8);
}
