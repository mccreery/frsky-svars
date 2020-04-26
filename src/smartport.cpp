#include <frsky/sport.hpp>
#include "protocol.hpp"
#include "endian.hpp"

namespace frsky::sport {

void putvar(std::ostream& stream, int channel, int32_t value) {
    write_var_packet(stream, channel, VarType::integer, value);
}

void putvar(std::ostream& stream, int channel, FixedPoint value) {
    write_var_packet(stream, channel, VarType::fixed_point, value.encode());
}

void putvar(std::ostream& stream, int channel, float value) {
    write_var_packet(stream, channel, VarType::floating_point, reinterpret_cast<uint32_t&>(value));
}

void putvar(std::ostream& stream, int channel, std::string value) {
    // Write string in chunks of 4 bytes
    for (unsigned i = 0; i < value.length(); i += 4) {
        unsigned chunk_size;
        if (i + 3 < value.length()) {
            chunk_size = 4;
        } else {
            chunk_size = value.length() - i;
        }

        uint32_t chunk = get_big_endian<uint32_t>(&value.c_str()[i], chunk_size);
        write_var_packet(stream, channel, VarType::string, chunk);
    }
}

static void write_var_packet(std::ostream& stream, int channel, VarType var_type, uint32_t value) {
    write_packet(stream, PhysicalID::vario, FrameHeader::sensor,
        get_data_id(channel, var_type), value);
}

static void write_packet(std::ostream& stream, PhysicalID physical_id, FrameHeader frame_header, DataID data_id, uint32_t value) {
    char packet[8];
    packet[0] = physical_id;
    packet[1] = frame_header;
    put_big_endian(&packet[2], data_id);
    put_big_endian(&packet[4], value);

    write_packet(stream, packet, sizeof(packet));
}

static void write_packet(std::ostream& stream, const char* packet, int size) {
    stream.write(packet, size);
    stream.put(get_crc(packet, size));
}

}
