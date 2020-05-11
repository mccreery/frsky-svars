#pragma once
#include <cstdint>
#include <string>
#include <serial/serial.h>
#include <vector>

/**
 * S.Vars root namespace.
 */
namespace frsky::sport {

/**
 * Represents a 24.8 fixed point number in two's complement.
 */
class FixedPoint {
public:
    /**
     * Conversion constructor from a signed integer.
     * The fractional part becomes zero.
     */
    FixedPoint(int32_t signed_) : mantissa(
        (uint32_t)(signed_ & 0x007fffff) << 8 | (signed_ & 0x80000000)
    ) {}
    /**
     * Conversion constructor from an unsigned integer.
     * The fractional part becomes zero.
     */
    FixedPoint(uint32_t unsigned_) : mantissa(unsigned_ << 8) {}
    /**
     * Conversion constructor from a float. The result should be identical if
     * the float is perfectly representable by 24.8.
     */
    FixedPoint(float float_) : mantissa(
        (int32_t)(float_ * 256)
    ) {}

    /**
     * @return The 32-bit S.Vars encoded value.
     */
    int32_t encode() { return mantissa; }
private:
    int32_t mantissa;
};

/**
 * @brief Sends an integer value over the serial port.
 *
 * @param serial The S.Port serial connection
 * @param channel The channel from 0 to 63
 * @param value The value
 */
void putvar(serial::Serial& serial, int channel, int32_t value);
/**
 * @brief Sends a fixed point value over the serial port.
 *
 * @param serial The S.Port serial connection
 * @param channel The channel from 0 to 63
 * @param value The value
 */
void putvar(serial::Serial& serial, int channel, FixedPoint value);
/**
 * @brief Sends a floating point value over the serial port.
 *
 * @param serial The S.Port serial connection
 * @param channel The channel from 0 to 63
 * @param value The value
 */
void putvar(serial::Serial& serial, int channel, float value);
/**
 * @brief Sends a text string over the serial port.
 *
 * @param serial The S.Port serial connection
 * @param channel The channel from 0 to 63
 * @param value The text string, ASCII compatible
 */
void putvar(serial::Serial& serial, int channel, std::string value);

/**
 * @brief Creates an integer packet without sending it.
 *
 * @param channel The channel from 0 to 63
 * @param value The value
 * @return std::vector<uint8_t> Sequence of bytes representing the packet
 */
std::vector<uint8_t> serialize(int channel, int32_t value);
/**
 * @brief Creates a fixed point packet without sending it.
 *
 * @param channel The channel from 0 to 63
 * @param value The value
 * @return std::vector<uint8_t> Sequence of bytes representing the packet
 */
std::vector<uint8_t> serialize(int channel, FixedPoint value);
/**
 * @brief Creates a floating point packet without sending it.
 *
 * @param channel The channel from 0 to 63
 * @param value The value
 * @return std::vector<uint8_t> Sequence of bytes representing the packet
 */
std::vector<uint8_t> serialize(int channel, float value);
/**
 * @brief Creates a string packet without sending it.
 *
 * @param channel The channel from 0 to 63
 * @param value The text string
 * @return std::vector<uint8_t> Sequence of bytes representing the packet
 */
std::vector<uint8_t> serialize(int channel, std::string value);

/**
 * @brief Configure a serial port for S.Port communication.
 * @param serial The unopened serial port for setup.
 */
void configure_serial(serial::Serial& serial);

}
