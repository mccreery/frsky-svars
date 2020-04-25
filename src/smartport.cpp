#include <smartport.hpp>
#include "protocol.hpp"

namespace smartport {

static DataID data_id(DataID var_type, int channel) {
    return (DataID)(var_type | (channel & 0x0fff));
}

void SmartPort::passthrough(int channel, int32_t data) {
    write_packet(FrameHeader::sensor, data_id(DataID::var_int, channel), data);
}

void SmartPort::passthrough(int channel, FixedPoint data) {
    write_packet(FrameHeader::sensor, data_id(DataID::var_fixed, channel), data.encode());
}

void SmartPort::passthrough(int channel, float data) {
    write_packet(FrameHeader::sensor, data_id(DataID::var_float, channel), reinterpret_cast<uint32_t&>(data));
}

void SmartPort::passthrough(int channel, std::string data) {
    // Round up length to multiple of 3
    unsigned num_packets = (data.length() + 2) / 3;

    for (int i = 0; num_packets > 0; i += 3) {
        --num_packets;

        write_packet(FrameHeader::sensor, data_id(DataID::var_string, channel),
            num_packets << 24
            | data[i] << 16
            | data[i + 1] << 8
            | data[i + 2]);
    }
}

void SmartPort::write_packet(FrameHeader frame_header, DataID data_id, uint32_t data) {
    // TODO assuming little endian byte order
    const char packet[7] = {
        frame_header,
        (char)data_id,
        (char)(data_id >> 8),
        (char)data,
        (char)(data >> 8),
        (char)(data >> 16),
        (char)(data >> 24)
    };
    write_packet(packet, sizeof(packet));
}

void SmartPort::write_packet(const char* packet, int size) {
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

}
