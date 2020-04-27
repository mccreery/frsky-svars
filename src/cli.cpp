#include <frsky/sport.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <readline/readline.h>
#include <serial/serial.h>

#include <csignal>
#include <csetjmp>

using namespace frsky;
typedef void (* TypeHandler)(serial::Serial&, int, std::string&);

class SyntaxError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

static std::jmp_buf env;
static void handle_exit_signal(int signal) {
    // Terminate normally
    (void)signal;
    std::longjmp(env, 0);
}

static void register_signal_handler() {
    struct sigaction action;
    action.sa_handler = handle_exit_signal;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}

static void handle_int(serial::Serial& stream, int channel, std::string& value) {
    int32_t int_value;
    try {
        int_value = std::stoi(value);
    } catch (std::invalid_argument& err) {
        throw SyntaxError("Invalid value of type int");
    }

    sport::putvar(stream, channel, int_value);
}

static void handle_fixed(serial::Serial& stream, int channel, std::string& value) {
    float float_value;
    try {
        float_value = std::stof(value);
    } catch (std::invalid_argument& err) {
        throw SyntaxError("Invalid value of type fixed");
    }

    sport::putvar(stream, channel, sport::FixedPoint(float_value));
}

static void handle_float(serial::Serial& stream, int channel, std::string& value) {
    float float_value;
    try {
        float_value = std::stof(value);
    } catch (std::invalid_argument& err) {
        throw SyntaxError("Invalid value of type float");
    }

    sport::putvar(stream, channel, float_value);
}

static void handle_string(serial::Serial& stream, int channel, std::string& value) {
    sport::putvar(stream, channel, value);
}

static std::map<std::string, TypeHandler> type_handlers {
    { "int", handle_int },
    { "fixed", handle_fixed },
    { "float", handle_float },
    { "string", handle_string }
};

static void process_line(serial::Serial& stream, std::string& line) {
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

    buf >> std::ws;
    std::getline(buf, value);
    if (!buf) {
        throw SyntaxError("Expected value");
    }

    type_handler(stream, channel, value);
    std::cout << "Packet sent" << std::endl;
}

int main(int argc, char** argv) {
    register_signal_handler();
    std::cout << "S.Port CLI v" << PROJECT_VERSION << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return 1;
    }

    serial::Serial serial;
    sport::configure_serial(serial);
    serial.setPort(argv[1]);

    try {
        serial.open();
    } catch (serial::IOException ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // when longjmp is called, setjmp returns 1
    while (!setjmp(env)) {
        std::string line(readline("> "));
        if (line == "q") return 0;

        try {
            process_line(serial, line);
        } catch(SyntaxError& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << "Command format: [CHANNEL (0-63)] [TYPE (int, fixed, float, string)] [VALUE]" << std::endl;
        }
    }
    // Extra newline when terminating unusually
    std::cout << std::endl;
}
