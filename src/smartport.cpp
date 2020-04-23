#include <smartport.hpp>
#include "protocol.hpp"

namespace smartport {

void SmartPort::SendData(uint16_t type, uint32_t data) {
    
}

/**
 * @brief Sends a single byte to the underlying stream.
 * Protocol reserved bytes are "byte-stuffed."
 * 
 * @param byte The raw data byte.
 */
void SmartPort::SendU8(uint8_t byte) {
    UpdateCrc(byte);

    if (byte == STUFFING || byte == SEPARATOR) {
        stream.write(STUFFING);
        byte ^= 0x60;
    }
    stream.write(byte);
}

void SmartPort::SendU16(uint16_t u16) {
    SendU8(u16);
    SendU8(u16 >> 8);
}

void SmartPort::UpdateCrc(uint8_t byte) {
    uint16_t crc16 = crc + byte;
    // Cycle
    crc16 += crc16 >> 8;
    // Discard upper byte
    crc = crc16;
}

}
