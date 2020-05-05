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

TEST(FixedPoint, Float) {
    EXPECT_EQ(sport::FixedPoint(0.0f).encode(), 0);
    EXPECT_EQ(sport::FixedPoint(31.25f).encode(), (signed)0x00001f40);
    EXPECT_EQ(sport::FixedPoint(-1.0f).encode(), (signed)0xffffff00);
    EXPECT_EQ(sport::FixedPoint(-0.5f).encode(), (signed)0xffffff80);
}
