#pragma once
#include <cstdint>
#include <iostream>

class SmartPort {
public:
    SmartPort(std::ostream& stream);
    void SendData(uint16_t type, uint32_t data);
private:
    void SendByte(uint8_t byte);
    void UpdateCrc(uint8_t byte);

    std::ostream& stream;
    uint8_t crc;
}

void sendData(ostream& out, uint16_t data);
