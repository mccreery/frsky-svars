#include <frsky/sport.hpp>
#include "protocol.hpp"
#include "endian.hpp"

namespace frsky::sport {

static DataID data_id(DataID var_type, int channel) {
    return (DataID)(var_type | (channel & 0x3f));
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
    // Write string in chunks of 4 bytes
    for (unsigned i = 0; i < data.length(); i += 4) {
        unsigned chunk_size;
        if (i + 3 < data.length()) {
            chunk_size = 4;
        } else {
            chunk_size = data.length() - i;
        }

        uint32_t chunk = get_big_endian<uint32_t>(&data.c_str()[i], chunk_size);
        write_packet(FrameHeader::sensor, data_id(DataID::var_string, channel), chunk);
    }
}

void SmartPort::write_packet(FrameHeader frame_header, DataID data_id, uint32_t data) {
    char packet[7];
    packet[0] = frame_header;
    put_big_endian(&packet[1], data_id);
    put_big_endian(&packet[3], data);

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
