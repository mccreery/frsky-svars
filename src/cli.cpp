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

/**
 * @brief Parameter function for sending a value based on type
 * @param serial The open S.Port serial connection
 * @param channel S.Vars channel 0-63
 * @param value Unconverted string value
 */
typedef void (* TypeHandler)(serial::Serial&, int, std::string&);

/**
 * Basic syntax error exception for echoing to the user
 */
class SyntaxError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

// Code for Ctrl+C signal handling - exit gracefully

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

// End signal handling

/** Sends a single int value */
static void handle_int(serial::Serial& stream, int channel, std::string& value) {
    int32_t int_value;
    try {
        int_value = std::stoi(value);
    } catch (std::invalid_argument& err) {
        throw SyntaxError("Invalid value of type int");
    }

    sport::putvar(stream, channel, int_value);
}

/** Sends a single fixed point value */
static void handle_fixed(serial::Serial& stream, int channel, std::string& value) {
    float float_value;
    try {
        float_value = std::stof(value);
    } catch (std::invalid_argument& err) {
        throw SyntaxError("Invalid value of type fixed");
    }

    sport::putvar(stream, channel, sport::FixedPoint(float_value));
}

/** Sends a single floating point value */
static void handle_float(serial::Serial& stream, int channel, std::string& value) {
    float float_value;
    try {
        float_value = std::stof(value);
    } catch (std::invalid_argument& err) {
        throw SyntaxError("Invalid value of type float");
    }

    sport::putvar(stream, channel, float_value);
}

/** Sends a single string value, could be multiple packets */
static void handle_string(serial::Serial& stream, int channel, std::string& value) {
    sport::putvar(stream, channel, value);
}

// Lookup specific keywords for sending each type
static std::map<std::string, TypeHandler> type_handlers {
    { "int", handle_int },
    { "fixed", handle_fixed },
    { "float", handle_float },
    { "string", handle_string }
};

/**
 * @brief Process a command by the user
 *
 * @param stream The open S.Port serial connection
 * @param line The user's command line
 */
static void process_line(serial::Serial& stream, std::string& line) {
    // Setup for consuming string
    std::istringstream buf;
    buf.str(line);
    buf >> std::skipws;

    // Parse >[channel] [type] [value]
    int channel;
    TypeHandler type_handler = nullptr;
    std::string value;

    buf >> channel;
    if (!buf || channel < 0 || channel >= 64) {
        throw SyntaxError("Expected channel (0-63)");
    }

    // Parse [channel] >[type] [value]
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

    // Parse [channel] [type] >[value]
    // Gobbles rest of line for string to work
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
    // Welcome message
    std::cout << "S.Port CLI v" << PROJECT_VERSION << std::endl;

    // No serial port specified
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [FILE]" << std::endl;
        return 1;
    }

    // Try to prepare S.Port connection
    serial::Serial serial;
    sport::configure_serial(serial);
    serial.setPort(argv[1]);

    try {
        serial.open();
    } catch (serial::IOException ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    // when longjmp is called, setjmp returns 1 and breaks loop
    while (!setjmp(env)) {
        // Prompt and process result
        // readline library adds much better text editing support
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
