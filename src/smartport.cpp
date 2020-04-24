#include <smartport.hpp>
#include "protocol.hpp"

namespace smartport {

void SmartPort::passthrough(int channel, int32_t data) {
    write_packet(FrameHeader::sensor, DataID::var_int | channel & 0x0fff, data);
}

void SmartPort::passthrough(int channel, FixedPoint data) {
    write_packet(FrameHeader::sensor, DataID::var_fixed | channel & 0x0fff, data.encode());
}

void SmartPort::passthrough(int channel, float data) {
    write_packet(FrameHeader::sensor, DataID::var_float | channel & 0x0fff, reinterpret_cast<float>(data));
}

void SmartPort::passthrough(int channel, std::string data) {
    // Round up length to multiple of 3
    int num_packets = (data.length() + 2) / 3;

    for (int i = 0; num_packets > 0; i += 3) {
        --num_packets;

        write_packet(FrameHeader::sensor, DataID::var_string | channel & 0x0fff,
            num_packets << 24
            | data[i] << 16
            | data[i + 1] << 8
            | data[i + 2]);
    }
}

void SmartPort::write_packet(FrameHeader frame_header, DataID data_id, uint32_t data) {
    // TODO assuming little endian byte order
    uint8_t packet[7] = {
        frame_header,
        data_id,
        data_id >> 8,
        data,
        data >> 8,
        data >> 16,
        data >> 24
    };
    write_packet(packet, sizeof(packet));
}

void SmartPort::write_packet(uint8_t[] packet, int size) {
    stream.write(packet, size);

    // Calculate CRC
    uint16_t crc = 0;
    for (int i = 0; i < size; i++) {
        crc += packet[i];
        crc += crc >> 8;
        crc &= 0xff;
    }
    stream.put(~crc);
}



void SmartPort::SendData(uint16_t type, uint8_t data[4]) {
    crc = 0;
    SendU8(DATA_FRAME_HEADER);
    SendU16(type);

    for (int i = 0; i < 4; i++) {
        SendU8(data[i]);
    }
    SendU8(~crc);
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
