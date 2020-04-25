#include <smartport.hpp>

int main(int argc, char** argv) {
    // Unused parameters
    (void)argc;
    (void)argv;

    smartport::SmartPort port(std::cout);
    // 31 bit positive number, 32 bit negative number
    port.passthrough(0, (signed)0x76543210);
    port.passthrough(1, (signed)0x87654321);

    port.passthrough(2, smartport::FixedPoint(-24.2625f));
    port.passthrough(3, smartport::FixedPoint(-32));

    // Extreme float values
    port.passthrough(4, 1.5e10f);
    port.passthrough(5, -3.25e-10f);

    // String containing UTF-8 Unicode
    port.passthrough(0, "Hello, world!");
}
