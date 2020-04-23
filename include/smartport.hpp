#pragma once
#include <cstdint>
#include <iostream>

namespace smartport {

class SmartPort {
public:
    SmartPort(std::ostream& stream);
    void SendData(uint16_t type, uint32_t data);
private:
    void SendU8(uint8_t byte);
    void SendU16(uint16_t data);
    void UpdateCrc(uint8_t byte);

    std::ostream& stream;
    uint8_t crc;
}

}
