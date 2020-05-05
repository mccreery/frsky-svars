#pragma once
#include <cstdint>
#include <string>
#include <serial/serial.h>
#include <vector>

namespace frsky::sport {

class FixedPoint {
public:
    FixedPoint(int32_t signed_) : mantissa(
        (uint32_t)(signed_ & 0x007fffff) << 8 | (signed_ & 0x80000000)
    ) {}
    FixedPoint(uint32_t unsigned_) : mantissa(unsigned_ << 8) {}
    FixedPoint(float float_) : mantissa(
        (int32_t)(float_ * 256)
    ) {}

    int32_t encode() { return mantissa; }
private:
    int32_t mantissa;
};

std::vector<uint8_t> serialize(int channel, int32_t value);
std::vector<uint8_t> serialize(int channel, FixedPoint value);
std::vector<uint8_t> serialize(int channel, float value);
std::vector<uint8_t> serialize(int channel, std::string value);

template <typename T>
void putvar(serial::Serial& serial, int channel, T value) {
    serial.write(serialize(channel, value));
}

template void putvar(serial::Serial&, int, int32_t);
template void putvar(serial::Serial&, int, FixedPoint);
template void putvar(serial::Serial&, int, float);
template void putvar(serial::Serial&, int, std::string);

void configure_serial(serial::Serial& serial);

}
