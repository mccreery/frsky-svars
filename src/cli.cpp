#include <frsky/sport.hpp>
#include <iostream>

using namespace frsky;

template <typename It>
static It check_equals(It begin, It end) {
    bool seen = false;
    It i;

    for (i = begin; i != end; i++) {
        if (*i == '=') {
            if (seen) {
                throw "Too many equals signs";
            } else {
                seen = true;
            }
        } else if (*i != ' ') {
            break;
        }
    }

    if (!seen) {
        throw "No equals sign";
    }
    return i;
}

static bool process_line(std::string& line) {
    if (line == "q") { return false; }

    int channel;
    size_t channel_length;
    try {
        channel = std::stoi(line, &channel_length);
    } catch (std::invalid_argument err) {
        throw "Invalid channel number";
    }

    auto i = line.cbegin() + channel_length;
    i = check_equals(i, line.cend());

    // TODO For testing
    std::cout << line.substr(i - line.cbegin()) << " on " << channel << std::endl;

    return true;
}

int main(int argc, char** argv) {
    (void)argv;
    std::cout << "S.Port CLI v" << PROJECT_VERSION << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return 1;
    }

    std::string line;
    bool next = true;

    do {
        std::cout << "> ";
        std::getline(std::cin, line);

        try {
            next = process_line(line);
        } catch (const char* const err) {
            std::cerr << err << std::endl;
        }
    } while (next);

    /*// 31 bit positive number, 32 bit negative number
    sport::putvar(std::cout, 0, (signed)0x76543210);
    sport::putvar(std::cout, 1, (signed)0x87654321);

    // Fixed point values
    sport::putvar(std::cout, 3, sport::FixedPoint(32));
    sport::putvar(std::cout, 2, sport::FixedPoint(-24.2625f));

    // Extreme float values
    sport::putvar(std::cout, 4, 1.5e10f);
    sport::putvar(std::cout, 5, -3.25e-10f);

    // String containing UTF-8 Unicode
    sport::putvar(std::cout, 6, "Hello, world!");*/
}
