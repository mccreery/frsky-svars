#pragma once
#include <cstdint>
#include <iostream>
#include <string>

namespace frsky::sport {

class FixedPoint {
public:
    FixedPoint(int32_t signed_) : mantissa(
        (uint32_t)(signed_ & 0x07ff) << 8 | (signed_ & 0x8000)
    ) {}
    FixedPoint(uint32_t unsigned_) : mantissa(unsigned_ << 8) {}
    FixedPoint(float float_) : FixedPoint((int32_t)(float_ * 256)) {}

    int32_t encode() { return mantissa; }
private:
    int32_t mantissa;
};

void putvar(std::ostream& stream, int channel, int32_t value);
void putvar(std::ostream& stream, int channel, FixedPoint value);
void putvar(std::ostream& stream, int channel, float value);
void putvar(std::ostream& stream, int channel, std::string value);

}
