#include <gtest/gtest.h>
#include <gmock/gmock.h>
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

TEST(Packets, Int) {
    uint8_t packet[] = {
        0x00, 0x10,
        0x51, 0x0f,
        0x00, 0x00, 0x30, 0x39,
        0x26 // checksum is just the sum if no overflow occurs, inverted
    };

    EXPECT_THAT(sport::serialize(15, 12345), testing::ElementsAreArray(
        packet, packet + sizeof(packet)));
}

TEST(Packets, Fixed) {
    uint8_t packet[] = {
        0x00, 0x10,
        0x51, 0x48,
        0x00, 0x00, 0x20, 0x00,
        0x36 // checksum is just the sum if no overflow occurs, inverted
    };

    EXPECT_THAT(sport::serialize(8, sport::FixedPoint(32)), testing::ElementsAreArray(
        packet, packet + sizeof(packet)));
}

TEST(Packets, Float) {
    uint8_t packet[] = {
        0x00, 0x10,
        0x51, 0x80,
        0xc0, 0x5b, 0x33, 0x33,
        0x9b // checksum is just the sum if no overflow occurs, inverted
    };

    EXPECT_THAT(sport::serialize(0, -3.425f), testing::ElementsAreArray(
        packet, packet + sizeof(packet)));
}

TEST(Packets, String) {
    uint8_t packets[] = {
        0x00, 0x10,
        0x51, 0xc0,
        'H', 'e', 'l', 'l',
        0x57, // checksum is just the sum if no overflow occurs, inverted

        0x00, 0x10,
        0x51, 0xc0,
        'o', ',', ' ', 'w',
        0xaa,

        0x00, 0x10,
        0x51, 0xc0,
        'o', 'r', 'l', 'd',
        0x2b,

        0x00, 0x10,
        0x51, 0xc0,
        '!', 0x00, 0x00, 0x00,
        0xbc
    };

    EXPECT_THAT(sport::serialize(0, "Hello, world!"), testing::ElementsAreArray(
        packets, packets + sizeof(packets)));
}
