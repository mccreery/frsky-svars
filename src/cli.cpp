#include <frsky/sport.hpp>

using namespace frsky;

int main(int argc, char** argv) {
    // Unused parameters
    (void)argc;
    (void)argv;

    // 31 bit positive number, 32 bit negative number
    sport::putvar(std::cout, 0, (signed)0x76543210);
    sport::putvar(std::cout, 1, (signed)0x87654321);

    // Fixed point values
    sport::putvar(std::cout, 3, sport::FixedPoint(32));
    sport::putvar(std::cout, 2, sport::FixedPoint(-24.2625f));

    // Extreme float values
    sport::putvar(std::cout, 4, 1.5e10f);
    sport::putvar(std::cout, 5, -3.25e-10f);

    // String containing UTF-8 Unicode
    sport::putvar(std::cout, 6, "Hello, world!");
}
