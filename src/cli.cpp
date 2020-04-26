#include <frsky/sport.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace frsky;
typedef void (* TypeHandler)(std::ostream&, int, std::string&);

class SyntaxError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

static void handle_int(std::ostream& stream, int channel, std::string& value) {
    int32_t int_value;
    try {
        int_value = std::stoi(value);
    } catch (std::invalid_argument& err) {
        throw SyntaxError("Invalid value of type int");
    }

    sport::putvar(stream, channel, int_value);
}

static std::map<std::string, TypeHandler> type_handlers {
    { "int", handle_int }
};

static void process_line(std::ostream& stream, std::string& line) {
    std::istringstream buf;
    buf.str(line);
    buf >> std::skipws;

    int channel;
    TypeHandler type_handler = nullptr;
    std::string value;

    buf >> channel;
    if (!buf || channel < 0 || channel >= 64) {
        throw SyntaxError("Expected channel (0-63)");
    }

    std::string type;
    buf >> type;
    if (buf) {
        auto i = type_handlers.find(type);
        if (i != type_handlers.end()) {
            type_handler = (*i).second;
        }
    }
    if (type_handler == nullptr) {
        throw SyntaxError("Expected type (int, fixed, float, string)");
    }

    buf >> value;
    if (!buf) {
        throw SyntaxError("Expected value");
    }

    type_handler(stream, channel, value);
    std::cout << "Packet sent" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "S.Port CLI v" << PROJECT_VERSION << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return 1;
    }

    // TODO temporary file instead of serial port
    std::ofstream serial(argv[1], std::ios::out | std::ios::binary);

    if (!serial.is_open() || serial.fail() || serial.bad()) {
        std::cerr << "Could not open output stream" << std::endl;
        return 1;
    }

    while (true) {
        std::cout << "> ";

        std::string line;
        std::getline(std::cin, line);
        if (line == "q") break;

        try {
            process_line(serial, line);
        } catch(SyntaxError& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << "Command format: [CHANNEL (0-63)] [TYPE (int, fixed, float, string)] [VALUE]" << std::endl;
        }
    }
}
