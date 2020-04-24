#pragma once
#include <cstdint>
#include <iostream>

namespace smartport {

// TODO: in 32 bits, stuff as much data into the packet as possible
// TODO: repair multiple packets into one long packet at the end

class FixedPoint {
public:
    FixedPoint(int32_t signed_) : mantissa(
        (uint32_t)(signed_ & 0x07ff) << 8 | signed_ & 0x8000
    ) {}
    FixedPoint(uint32_t unsigned_) : mantissa(unsigned_ << 8) {}
    FixedPoint(float float_) : FixedPoint((int32_t)(float_ * 256)) {}

    int32_t encode() { return mantissa; }
private:
    int32_t mantissa;
};

class SmartPort {
public:
    SmartPort(std::ostream& stream);

    void passthrough(int channel, int32_t data);
    void passthrough(int channel, FixedPoint data);
    void passthrough(int channel, float data);
    void passthrough(int channel, std::string data);
private:
    void write_packet(uint8_t frame_header, uint16_t data_id, uint32_t data);
    void write_packet(uint8_t[] packet);
    std::ostream& stream;
};

}
