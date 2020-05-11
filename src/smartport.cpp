#include <frsky/sport.hpp>
#include "protocol.hpp"
#include "endian.hpp"

// Check needed for sending standard floating point format
static_assert(std::numeric_limits<float>::is_iec559,
    "Code can only be compiled on platforms which use IEEE 754 floating point");

namespace frsky::sport {

void putvar(serial::Serial& serial, int channel, int32_t value) {
    serial.write(serialize(channel, value));
}

void putvar(serial::Serial& serial, int channel, FixedPoint value) {
    serial.write(serialize(channel, value));
}

void putvar(serial::Serial& serial, int channel, float value) {
    serial.write(serialize(channel, value));
}

void putvar(serial::Serial& serial, int channel, std::string value) {
    serial.write(serialize(channel, value));
}

std::vector<uint8_t> serialize(int channel, int32_t value) {
    return serialize_var_packet(channel, VarType::integer, value);
}

std::vector<uint8_t> serialize(int channel, FixedPoint value) {
    return serialize_var_packet(channel, VarType::fixed_point, value.encode());
}

std::vector<uint8_t> serialize(int channel, float value) {
    return serialize_var_packet(channel, VarType::floating_point, reinterpret_cast<uint32_t&>(value));
}

std::vector<uint8_t> serialize(int channel, std::string value) {
    // Store multiple packets in one vector
    std::vector<uint8_t> packets;

    // Write string in chunks of 4 bytes
    for (unsigned i = 0; i < value.length(); i += 4) {
        unsigned chunk_size;
        // Consume maximum 4 bytes of string
        if (i + 3 < value.length()) {
            chunk_size = 4;
        } else {
            chunk_size = value.length() - i;
        }

        // const char to const uint8_t (unsigned)
        const uint8_t* chunk_data = reinterpret_cast<const uint8_t*>(&value.c_str()[i]);
        // Pack bytes into chunk
        uint32_t chunk = get_big_endian<uint32_t>(chunk_data, chunk_size);

        std::vector<uint8_t> packet = serialize_var_packet(channel, VarType::string, chunk);
        packets.insert(packets.end(), packet.begin(), packet.end());
    }
    return packets;
}

std::vector<uint8_t> serialize_var_packet(int channel, VarType var_type, uint32_t value) {
    // sensor == 0x10 only value allowed by S.Vars decoder
    return serialize_packet(PhysicalID::vario, FrameHeader::sensor,
        get_data_id(channel, var_type), value);
}

std::vector<uint8_t> serialize_packet(PhysicalID physical_id, FrameHeader frame_header, DataID data_id, uint32_t value) {
    // Construct packet using array before vector
    uint8_t raw_packet[8];
    raw_packet[0] = physical_id;
    raw_packet[1] = frame_header;
    put_big_endian(&raw_packet[2], data_id);
    put_big_endian(&raw_packet[4], value);

    // CRC in 9th byte calculated from first 8
    std::vector<uint8_t> packet(raw_packet, raw_packet + 8);
    packet.push_back(get_crc(packet.begin(), packet.end()));

    return packet;
}

void configure_serial(serial::Serial& serial) {
    serial.setBaudrate(57600u);

    serial.setBytesize(serial::eightbits);
    serial.setParity(serial::parity_none);
    serial.setStopbits(serial::stopbits_one);
    serial.setFlowcontrol(serial::flowcontrol_none);
}

}
