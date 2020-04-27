#include <frsky/sport.hpp>
#include "protocol.hpp"
#include "endian.hpp"

namespace frsky::sport {

void putvar(serial::Serial& serial, int channel, int32_t value) {
    write_var_packet(serial, channel, VarType::integer, value);
}

void putvar(serial::Serial& serial, int channel, FixedPoint value) {
    write_var_packet(serial, channel, VarType::fixed_point, value.encode());
}

void putvar(serial::Serial& serial, int channel, float value) {
    write_var_packet(serial, channel, VarType::floating_point, reinterpret_cast<uint32_t&>(value));
}

void putvar(serial::Serial& serial, int channel, std::string value) {
    // Write string in chunks of 4 bytes
    for (unsigned i = 0; i < value.length(); i += 4) {
        unsigned chunk_size;
        if (i + 3 < value.length()) {
            chunk_size = 4;
        } else {
            chunk_size = value.length() - i;
        }

        const uint8_t* chunk_data = reinterpret_cast<const uint8_t*>(&value.c_str()[i]);
        uint32_t chunk = get_big_endian<uint32_t>(chunk_data, chunk_size);

        write_var_packet(serial, channel, VarType::string, chunk);
    }
}

static void write_var_packet(serial::Serial& serial, int channel, VarType var_type, uint32_t value) {
    write_packet(serial, PhysicalID::vario, FrameHeader::sensor,
        get_data_id(channel, var_type), value);
}

static void write_packet(serial::Serial& serial, PhysicalID physical_id, FrameHeader frame_header, DataID data_id, uint32_t value) {
    uint8_t packet[8];
    packet[0] = physical_id;
    packet[1] = frame_header;
    put_big_endian(&packet[2], data_id);
    put_big_endian(&packet[4], value);

    write_packet(serial, std::vector<uint8_t>(packet, packet + 8));
}

static void write_packet(serial::Serial& serial, const std::vector<uint8_t>& packet) {
    serial.write(packet);

    uint8_t crc = get_crc(packet.begin(), packet.end());
    serial.write(&crc, 1);
}

void configure_serial(serial::Serial& serial) {
    serial.setBaudrate(57600u);

    serial.setBytesize(serial::eightbits);
    serial.setParity(serial::parity_none);
    serial.setStopbits(serial::stopbits_one);
    serial.setFlowcontrol(serial::flowcontrol_none);
}

}
