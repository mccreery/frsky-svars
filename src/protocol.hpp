#pragma once
#include <cstdint>
#include <vector>

namespace frsky::sport {

enum PhysicalID : uint8_t {
    vario
    // Other physical IDs omitted as not relevant, we assume 0 is fine
    // vario value 0 sourced from:
    // https://github.com/opentx/opentx/blob/2.3/radio/src/telemetry/frsky.h#L211
};

enum FrameHeader : uint8_t {
    /**
     * Indicates a sensor packet. Packets with corresponding data IDs between
     * 0x5000 and 0x52ff are considered passthrough packets. Low IDs are
     * reserved for the official FrSky sensors.
     */
    sensor = 0x10,
    /**
     * Indicates a raw passthrough packet. OpenTX will ignore the data ID and
     * pass the packet directly to any Lua script.
     *
     * https://opentx.gitbooks.io/opentx-2-2-lua-reference-guide/content/general/sportTelemetryPop.html
     */
    passthrough = 0x32
};

/**
 * Data IDs defined here:
 * https://cdn.rawgit.com/ArduPilot/ardupilot_wiki/33cd0c2c/images/FrSky_Passthrough_protocol.xlsx
 */
enum DataID : uint16_t {
    // Unused for now
    text = 0x5000,
    ap_status = 0x5001,
    gps_status = 0x5002,
    battery = 0x5003,
    home = 0x5004,
    velocity_yaw = 0x5005,
    roll_pitch_range = 0x5006,
    params = 0x5007,

    // Application-defined IDs from here
    var_signature = 0x5100
};

enum VarType {
    integer,
    fixed_point,
    floating_point,
    string
};

static DataID get_data_id(int channel, VarType var_type) {
    return (DataID)(DataID::var_signature
        | (var_type << 6)
        | (channel & 0x3f));
}

template <typename It>
static uint8_t get_crc(It begin, It end) {
    uint16_t crc = 0;

    for (It i = begin; i != end; i++) {
        crc += *i;
        crc += crc >> 8;
        crc &= 0xff;
    }
    return (uint8_t)~crc;
}

static void write_var_packet(serial::Serial& stream, int channel, VarType var_type, uint32_t value);
static void write_packet(serial::Serial& stream, PhysicalID physical_id, FrameHeader frame_header, DataID data_id, uint32_t value);
static void write_packet(serial::Serial& stream, const std::vector<uint8_t>& packet);

}
